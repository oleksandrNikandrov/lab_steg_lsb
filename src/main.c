#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>

#include "../include/stego_lsb.h"
#include "../include/bmp.h"

// steg_lsb decode | encode

void print_help(const char *progname);

int main(int argc, char **argv) {
    char *input = NULL;
    char *output = NULL;
    char *msg_str = NULL;
    char *msg_file = NULL;
    char *mode = NULL;

    int opt;
    int long_index = 0;

    static struct option long_options[] = {
            {"help", no_argument, 0, 'h'},
            {"input", required_argument, 0, 'i'},
            {"output", required_argument, 0, 'o'},
            {"msg-file", required_argument, 0, 0},
            {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "i:o:m:h", long_options, &long_index)) != -1) {
        switch (opt) {
            case 'i': input = optarg; break;
            case 'o': output = optarg; break;
            case 'm': msg_str = optarg; break;
            case 'h': print_help(argv[0]); return 0;

            case 0:
                if (strcmp(long_options[long_index].name, "msg-file") == 0) {
                    msg_file = optarg;
                }
                break;
            default:
                print_help(argv[0]);
                return 1;
        }
    }

    if (optind >= argc) {
        LOG_ERROR("missing mode (encode/decode) ");
        print_help(argv[0]);
        return 1;
    }

    mode = argv[optind];

    BitStream bs;
    BMPImage *img = NULL;

    if (strcmp(mode, "encode") == 0) {
        if (!input || !output || (!msg_str && !msg_file)) {
            LOG_ERROR("encode requires -i <input.bmp> -o <output.bmp> -m <\"message\"> | --msg-file <message.txt> ");
            print_help(argv[0]);
            return 1;
        }

        char *msg_data = NULL;
        uint32_t msg_size = 0;

        if (NULL != msg_str){
            msg_data = msg_str;
            msg_size = strlen(msg_str);
        } else if (NULL != msg_file){

            FILE *mf = fopen(msg_file, "rb");
            if(NULL == mf){
                LOG_ERROR("occurs while opening message_file");
                return 1;
            }
            fseek(mf, 0, SEEK_END);
            msg_size = ftell(mf);
            fseek(mf, 0, SEEK_SET);

            msg_data = malloc(msg_size);
            if(NULL == msg_data) {
                LOG_ERROR("occurs while allocating memory for msg_data");
                fclose(mf);
                return 1;
            }

            fread(msg_data, 1, msg_size, mf);
            fclose(mf);
        }

        printf("Encoding message (%zu bytes) into %s → %s\n",
               msg_size, input, output);

        ERRStatus status = bmp_load(input, &img);

        switch(status){
            case ERR_FILE_NOT_FOUND: LOG_ERROR("there is no file with that name. quitting ... "); return 2;
            case MEMORY_ERR: LOG_ERROR("there are some issues with memory allocation. quitting ..."); return 3;
            case IO_ERROR: LOG_ERROR("there are some issues with reading file. quitting ... "); return 4;
            case ERR_INVALID_FORMAT: LOG_ERROR("invalid file format. please check BPP and COMPRESSION. quitting ... "); return 5;

            case OK_STATUS: LOG_ERROR("got a OK_STATUS from bmp_load()");
        }

        bs_init(&bs, img->pixels, img->data_size);

        status = encode_classic(&bs, msg_data, msg_size);

        switch( status ) {
            case MEMORY_ERR: return 3;

            case OK_STATUS: LOG_ERROR("got a OK_STATUS from encode_classic()");
        }

        status = bmp_save(output, &img);

        switch( status ) {
            case MEMORY_ERR: LOG_ERROR("there are some issues with memory allocation. quitting ..."); return 3;
            case ERR_FILE_NOT_FOUND: LOG_ERROR("there is no file with that name. quitting ... "); return 2;
            case IO_ERROR: LOG_ERROR("there are some issues with writing file. quitting ... "); return 4;

            case OK_STATUS: LOG_ERROR("got a OK_STATUS from bmp_save()");
        }

    }
    return 0;
}


void print_help(const char *progname)
{

    printf("Usage: %s <mode> [options]\n", progname);
    printf("\n");

    printf("Modes:\n");
    printf("  encode   Встраивание сообщения в BMP\n");
    printf("  decode   Извлечение сообщения из BMP\n");
    printf("\n");

    printf("Options:\n");
    printf("  -i, --input <file>       Входной BMP-файл\n");
    printf("  -o, --output <file>      Выходной BMP-файл (для encode)\n");
    printf("  -m, --message <string>   Сообщение для встраивания (строка)\n");
    printf("      --msg-file <file>    Файл с сообщением для встраивания\n");
    printf("  -h, --help               Показать эту справку\n");
    printf("\n");

    printf("Examples:\n");
    printf("  %s encode -i cover.bmp -o stego.bmp -m \"Hello\"\n", progname);
    printf("  %s encode -i cover.bmp -o stego.bmp --msg-file secret.txt\n", progname);
    printf("  %s decode -i stego.bmp\n", progname);
    printf("\n");

}
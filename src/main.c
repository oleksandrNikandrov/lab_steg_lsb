#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>

#include "../include/stego_lsb.h" // for BMPImage
#include "util/common.h" // for Option struct
#include "cli/alg_handlers.h"

void print_help(const char *progname);
ERRStatus parse_args(int32_t argc, char **argv, Options *opts);

int main(int argc, char **argv) {
    StegoContext ctx;

    ERRStatus status = parse_args(argc, argv, &(ctx.opts));

    switch( status ){
        case ERR_MISSING_MODE_OR_ALG: LOG_ERROR("missing <algorithm> and/or <mode> (expected: <algorithm> <mode>)"); return 6;
        case 1: print_help(argv[0]); return 0;
        case -1: print_help(argv[0]); return -1;

        case OK_STATUS: LOG_INFO("got a OK_STATUS from parse_args()");
    }

    for(uint8_t i = 0; handlers[i].name; i++){
        if(strcmp(handlers[i].name, ctx.opts.alg) == 0){
            handlers[i].handler(&ctx);
        }
    }


    if (strcmp(opts.mode, "encode") == 0) {
        if (!input || !output || (!msg_str && !msg_file)) {
            LOG_ERROR("encode requires -i <input.bmp> -o <output.bmp> -m <\"message\"> | --msg-file <message.txt> \n\n");
            print_help(argv[0]);
            return 1;
        }

        msg.data = NULL;
        msg.data_size = 0;

        if (NULL != msg_str){
            msg.data = msg_str;
            msg.data_size = strlen(msg_str);
        } else if (NULL != msg_file){

            FILE *mf = fopen(msg_file, "rb");
            if(NULL == mf){
                LOG_ERROR("occurs while opening message_file");
                return 1;
            }
            fseek(mf, 0, SEEK_END);
            msg.data_size = ftell(mf);
            fseek(mf, 0, SEEK_SET);

            msg.data = malloc(msg.data_size);
            if(NULL == msg.data) {
                LOG_ERROR("occurs while allocating memory for msg_data");
                fclose(mf);
                return 1;
            }

            fread(msg.data, 1, msg.data_size, mf);
            fclose(mf);
        }

        printf("Encoding message (%zu bytes) into %s → %s\n",
               msg.data_size, input, output);

        status = bmp_load(input, &img);

        switch( status ){
            case ERR_FILE_NOT_FOUND: LOG_ERROR("there is no file with that name. quitting ... "); return 2;
            case MEMORY_ERR: LOG_ERROR("there are some issues with memory allocation. quitting ..."); return 3;
            case IO_ERROR: LOG_ERROR("there are some issues with reading file. quitting ... "); return 4;
            case ERR_INVALID_FORMAT: LOG_ERROR("invalid file format. please check BPP and COMPRESSION. quitting ... "); return 5;

            case OK_STATUS: LOG_INFO("got a OK_STATUS from bmp_load()");
        }

        bs_init(&bs, img.pixels, img.data_size);

        status = encode_classic(&bs, &msg);

        switch( status ) {
            case MEMORY_ERR: return 3;

            case OK_STATUS: LOG_INFO("got a OK_STATUS from encode_classic()");
        }

        status = bmp_save(output, &img);

        switch( status ) {
            case MEMORY_ERR: LOG_ERROR("there are some issues with memory allocation. quitting ..."); return 3;
            case ERR_FILE_NOT_FOUND: LOG_ERROR("there is no file with that name. quitting ... "); return 2;
            case IO_ERROR: LOG_ERROR("there are some issues with writing file. quitting ... "); return 4;

            case OK_STATUS: LOG_INFO("got a OK_STATUS from bmp_save()");
        }

// ------------ decode -------------------------------

    } else if (strcmp(mode, "decode") == 0) {
        if( !input || !output ){
            LOG_ERROR("decode requires -i <input.bmp> -o <output.bmp> \n\n");
            print_help(argv[0]);
            return 1;
        }

        msg.data = NULL;
        msg.data_size = 0;

        ERRStatus status = bmp_load(input, &img);

        switch(status){
            case ERR_FILE_NOT_FOUND: LOG_ERROR("there is no file with that name. quitting ... "); return 2;
            case MEMORY_ERR: LOG_ERROR("there are some issues with memory allocation. quitting ..."); return 3;
            case IO_ERROR: LOG_ERROR("there are some issues with reading file. quitting ... "); return 4;
            case ERR_INVALID_FORMAT: LOG_ERROR("invalid file format. please check BPP and COMPRESSION. quitting ... "); return 5;

            case OK_STATUS: LOG_INFO("got a OK_STATUS from bmp_load()");
        }

        bs_init(&bs, img.pixels, img.data_size);

        status = decode_classic(&bs, &msg);

        switch( status ) {
            case MEMORY_ERR: LOG_ERROR("there are some issues with memory allocation. quitting ..."); return 3;

            case OK_STATUS: LOG_INFO("got a OK_STATUS from decode_classic()");
        }

        FILE *f = fopen(output, "w");
        if(NULL == f) {
            LOG_ERROR("there is no file with that name. quitting ... ");
            return 2;
        }

        fprintf(f, msg.data);

        fclose(f);
    }
    return 0;
}

ERRStatus parse_args(int32_t argc, char **argv, Options *opts){
    int opt, long_index = 0;

    static struct option long_options[] = {
            {"help", no_argument, 0, 'h'},
            {"input", required_argument, 0, 'i'},
            {"output", required_argument, 0, 'o'},
            {"msg-file", required_argument, 0, 0},
            {0, 0, 0, 0}
    };

    while ((opt = getopt_long(argc, argv, "i:o:m:h", long_options, &long_index)) != -1) {
        switch ( opt ) {
            case 'i': opts->input = optarg; break;
            case 'o': opts->output = optarg; break;
            case 'm': opts->msg_str = optarg; break;
            case 'h': return 1; // to print help

            case 0:
                if (strcmp(long_options[long_index].name, "msg-file") == 0) {
                    opts->msg_file = optarg;
                }
                break;
            default: return -1;
        }
    }

    if (optind + 1 >= argc) return ERR_MISSING_MODE_OR_ALG;

    opts->alg = argv[optind];
    opts->mode = argv[optind + 1];

    return OK_STATUS;
}


void print_help(const char *progname)
{

    printf("Usage: %s <algorithm> <mode> [options]\n", progname);
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
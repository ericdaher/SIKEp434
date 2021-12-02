/********************************************************************************************
* Abstract: run tests against known answer test vectors
*
* Modified from a file created by Bassham, Lawrence E (Fed) on 8/29/17.
* Copyright © 2017 Bassham, Lawrence E (Fed). All rights reserved.
*********************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "rng.h"
#include "api.h"

/* 
Os arquivos "clock_config.h" e "MKKL25Z4.h" são gerados pela IDE MCUXpresso automaticamente quando um projeto para a placa FDRM-KL25Z é criado.
São usados para configurar o SysTick, usados para medição dos ciclos de CPU das operações de geração de chaves, encriptação e decriptação.
Assim, é possível executar o código corretamente simplesmente removendo essas inclusões e as linhas relacionadas ao SysTick, caso necessário.
*/

#include "clock_config.h"
#include "MKL25Z4.h"

#define MAX_MARKER_LEN      50
#define KAT_SUCCESS          0
#define KAT_CRYPTO_FAILURE  -4
#define KAT_VERIFICATION_ERROR -2
#define MSG_BYTES               16

int     ReadHex(unsigned char *A, char str_to_convert[], int Length);

volatile uint32_t countdownTicks = 0;

void SysTick_Handler(void) {
	countdownTicks++;
	SysTick_Config(16000000);
}

int
main()
{
    unsigned char       seed[48];
    unsigned char       ct[CRYPTO_CIPHERTEXTBYTES], ss[CRYPTO_BYTES], ss1[CRYPTO_BYTES]; //, ct_rsp[CRYPTO_CIPHERTEXTBYTES], ss_rsp[CRYPTO_BYTES];
    unsigned char       pk[CRYPTO_PUBLICKEYBYTES], sk[CRYPTO_SECRETKEYBYTES]; //, pk_rsp[CRYPTO_PUBLICKEYBYTES], sk_rsp[CRYPTO_SECRETKEYBYTES];
    int                 ret_val;
    volatile uint32_t cnt=0;
    uint16_t ret;

    char seed_hex[] = "2A6F7386B815366F572AEB6C79E272CC21B7095FE09575F18072C9D677DA23BC9C8A4BC393B7524604D299BEDD260C8B";
    // char pk_hex[] = "D10B725FA678E2B06C026AF41AB8728C777C643D959AFF59DD51698CF563D171F3187C6BFCCCF8764CCEBAA6508B0EE619912EEE8C5D3061FB67B383D5DF2B9A40B7023A5D1459757EFEB3638A6834DB338AB82D3B6BFDDD498770ACFBF9CEE3ED01D6990A80A2275D19095C08E456AF201E3EBE266AC5120E7A32CEC4323F43A9E29A442EE3E29E80658F20CDBB733E80940A88EC9C07843AC7E99EFC9DD27DB34E21A5F641AC94B2B95F7CC846CEE3B3ED74D041805381E65D9F1229F11A3EBCD2B3F04C5FC6E7FA87B933138EBE473E47C6B05B829596133F42ED4BF315164B31C524B82532EB0B84E547C5649DC542B017F43460C78F10834737054AA600E6419397DD0F0D594221BB64A429E82786667B17EFA10AFA58FF5DE963D825FFD6F93A5FC9E4741C91BA79FB5C4AF49E90431E0262BD6D334E4039C67F47496690906736D3ED3138DFA9A3817BF8886F64BFB686BDB1FFD20639525CA9BBC2DD5F57D494E20A88591BB7DBE887AB1E89BCA517BE5572DC9F3F40";
    // char sk_hex[] = "195D6C86A3DF4C21E3007D7F2768B43C74CB3060E0ECA77FAE77E0F9F21EABD8C0C6EEA7767F4E10FDE5C2D79B8400BF96B19014B457EC01D10B725FA678E2B06C026AF41AB8728C777C643D959AFF59DD51698CF563D171F3187C6BFCCCF8764CCEBAA6508B0EE619912EEE8C5D3061FB67B383D5DF2B9A40B7023A5D1459757EFEB3638A6834DB338AB82D3B6BFDDD498770ACFBF9CEE3ED01D6990A80A2275D19095C08E456AF201E3EBE266AC5120E7A32CEC4323F43A9E29A442EE3E29E80658F20CDBB733E80940A88EC9C07843AC7E99EFC9DD27DB34E21A5F641AC94B2B95F7CC846CEE3B3ED74D041805381E65D9F1229F11A3EBCD2B3F04C5FC6E7FA87B933138EBE473E47C6B05B829596133F42ED4BF315164B31C524B82532EB0B84E547C5649DC542B017F43460C78F10834737054AA600E6419397DD0F0D594221BB64A429E82786667B17EFA10AFA58FF5DE963D825FFD6F93A5FC9E4741C91BA79FB5C4AF49E90431E0262BD6D334E4039C67F47496690906736D3ED3138DFA9A3817BF8886F64BFB686BDB1FFD20639525CA9BBC2DD5F57D494E20A88591BB7DBE887AB1E89BCA517BE5572DC9F3F40";
    // char ct_hex[] = "F80525D9BE49304D5C07F0B9A916080CEC3CF99215327586451511E8FB978C5CAF64CDA28AB3A1A1D5A09BFCB9E67CC2D3131E9072EA717A99CA4E461B1C310EA1F1D92E1C40E0E8B0C907C8556CF1D840B41F4A6D95942DEDAF033AC2F8A7337654638E81BC0169856578DC858299D86876676411F613F53E1B4D4E1E31E7C6F9050966BC9FBABD40E92C76E5B1EE0DAF99C92B7B8F5265D371B237213317D4503C1064325C3B487E2A26BE76A4C51C8B3865A567947AEFB433A2FB39AE3D33D8402ADFC9CE470E6CE120BBD73232EFC6905BE5F665A56AE2C66C1FB596ABB4814D2C7E9469E3D7310295274355AA2A0A7B6217A55946B67DD8C2074368A4B00ABB0347F60129A4DD57CBD64CC7B6B6D8CBE6F02BA1E7618632F9BFDEDF4EA198DE61A6AF1E1450CD138FEBC0B392E0A267B8028C2F150AE5DF389A9041CE66D0E6CA81703F7176E357E558903EA0F2AF1E6C50A69CC46A5A2B8C2F90DDDD28B56450595BEE431CCE00D2504CA556C5C2281C20E54701C85E1D61A7B827E13D7172E3D6AF50711D70F62ADB258EE986B198";
    // char ss_hex[] = "A047B7E873C8EA4ECE4B143C3E0896E9E9C7863511508292";

    BOARD_BootClockRUN();

    ReadHex(seed, seed_hex, 48);
    // ReadHex(pk_rsp, pk_hex, CRYPTO_PUBLICKEYBYTES);
    // ReadHex(sk_rsp, sk_hex, CRYPTO_SECRETKEYBYTES);
    // ReadHex(ss_rsp, ss_hex, CRYPTO_BYTES);
    // ReadHex(ct_rsp, ct_hex, CRYPTO_CIPHERTEXTBYTES);

    // randombytes_init(seed, NULL, 256);
    randombytes(sk, MSG_BYTES);

    SysTick_Config(16000000);
    // Generate the public/private keypair
    if ( (ret_val = crypto_kem_keypair(pk, sk)) != 0) {
        printf("crypto_kem_keypair returned <%d>\n", ret_val);
        return KAT_CRYPTO_FAILURE;
    }
    cnt = 16000000 - SysTick->VAL;

    if ( (ret_val = crypto_kem_enc(ct, ss, pk)) != 0) {
        printf("crypto_kem_enc returned <%d>\n", ret_val);
        return KAT_CRYPTO_FAILURE;
    }
    cnt = 16000000 - SysTick->VAL;

    if ( (ret_val = crypto_kem_dec(ss1, ct, sk)) != 0) {
        printf("crypto_kem_dec returned <%d>\n", ret_val);
        return KAT_CRYPTO_FAILURE;
    }
    cnt = 16000000 - SysTick->VAL;

    if ( memcmp(ss, ss1, CRYPTO_BYTES) ) {
        printf("crypto_kem_dec returned bad 'ss' value\n");
        return KAT_CRYPTO_FAILURE;
    }

    return KAT_SUCCESS;
}

int
ReadHex(unsigned char *A, char str_to_convert[], int Length)
{
    int              i, j;
    unsigned char    ich;

    if ( Length == 0 ) {
        A[0] = 0x00;
        return 1;
    }
    memset(A, 0x00, Length);

    j = 0;
    while (j < strlen(str_to_convert)) {
        if ( (str_to_convert[j] >= '0') && (str_to_convert[j] <= '9') )
            ich = str_to_convert[j] - '0';
        else if ( (str_to_convert[j] >= 'A') && (str_to_convert[j] <= 'F') )
            ich = str_to_convert[j] - 'A' + 10;
        else if ( (str_to_convert[j] >= 'a') && (str_to_convert[j] <= 'f') )
            ich = str_to_convert[j] - 'a' + 10;
        else // shouldn't ever get here
            ich = 0;

        for ( i=0; i<Length-1; i++ )
            A[i] = (A[i] << 4) | (A[i+1] >> 4);
        A[Length-1] = (A[Length-1] << 4) | ich;

        j++;
    }

    return 1;
}


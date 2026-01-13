#include "./crypt/crypt_aes_test.h" 
#include <stdio.h>
#include "./sys/sysio.h"




//#include "shell.h"


#include <string.h>
#include <stdint.h>


//有这两条语句，会去除编译警告
#include "mbedtls/aes.h"
#include "mbedtls/cipher.h"



#if 0

// PKCS#7 填充
int pkcs7_pad(const unsigned char *input, size_t input_len, 
              unsigned char **output, size_t *output_len) {
    int pad_len = 16 - (input_len % 16);
    *output_len = input_len + pad_len;
    *output = (unsigned char *)malloc(*output_len);
    if (!*output) return -1;
    memcpy(*output, input, input_len);
    memset(*output + input_len, pad_len, pad_len);
    return 0;
}

// PKCS#7 去除填充
int pkcs7_unpad(const unsigned char *input, size_t input_len,
                unsigned char **output, size_t *output_len) {
    if (input_len == 0 || input_len % 16 != 0) return -1;
    unsigned char pad_len = input[input_len - 1];
    if (pad_len < 1 || pad_len > 16) return -1;
    for (size_t i = input_len - pad_len; i < input_len; i++) {
        if (input[i] != pad_len) return -1;
    }
    *output_len = input_len - pad_len;
    *output = (unsigned char *)malloc(*output_len);
    if (!*output) return -1;
    memcpy(*output, input, *output_len);
    return 0;
}

// AES-ECB 加密
int aes_ecb_encrypt(const unsigned char *key, size_t key_bits,
                    const unsigned char *input, size_t input_len,
                    unsigned char **output, size_t *output_len) {
    mbedtls_aes_context aes_ctx;
    unsigned char *padded_input = NULL;
    size_t padded_len;

    if (pkcs7_pad(input, input_len, &padded_input, &padded_len) != 0) return -1;

    *output = (unsigned char *)malloc(padded_len);
    if (!*output) {
        free(padded_input);
        return -1;
    }
    *output_len = padded_len;

    mbedtls_aes_init(&aes_ctx);
    if (mbedtls_aes_setkey_enc(&aes_ctx, key, key_bits) != 0) {
        free(padded_input);
        free(*output);
        return -1;
    }

    for (size_t i = 0; i < padded_len; i += 16) {
        mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_ENCRYPT, 
                             padded_input + i, *output + i);
    }

    mbedtls_aes_free(&aes_ctx);
    free(padded_input);
    return 0;
}

// AES-ECB 解密
int aes_ecb_decrypt(const unsigned char *key, size_t key_bits,
                    const unsigned char *input, size_t input_len,
                    unsigned char **output, size_t *output_len) {
    mbedtls_aes_context aes_ctx;
    unsigned char *decrypted_data = NULL;
    unsigned char *unpadded_data = NULL;

    if (input_len % 16 != 0) return -1;

    decrypted_data = (unsigned char *)malloc(input_len);
    if (!decrypted_data) return -1;

    mbedtls_aes_init(&aes_ctx);
    if (mbedtls_aes_setkey_dec(&aes_ctx, key, key_bits) != 0) {
        free(decrypted_data);
        return -1;
    }

    for (size_t i = 0; i < input_len; i += 16) {
        mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_DECRYPT,
                             input + i, decrypted_data + i);
    }

    if (pkcs7_unpad(decrypted_data, input_len, &unpadded_data, output_len) != 0) {
        mbedtls_aes_free(&aes_ctx);
        free(decrypted_data);
        return -1;
    }

    *output = unpadded_data;
    mbedtls_aes_free(&aes_ctx);
    free(decrypted_data);
    return 0;
}

// 打印十六进制数据
//void print_hex(const char *label, const unsigned char *data, size_t len) {
//    SYSTEM_INFO("%s (length %zu): ", label, len);
//    for (size_t i = 0; i < len; i++) {
//        SYSTEM_INFO("%02X ", data[i]);
//    }
//    SYSTEM_INFO("\r\n");
//}
//success
int aesmain() {
    const unsigned char key[] = "0123456789ABCDEF"; // 16字节密钥
   // const char *plaintext = "Hello, ECB Mode!Hello, ECB Mode!"; //strlen    // 明文长度16字节（无需填充）
	const char plaintext[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x25, 0x22, 0x40, 0x9c, 0xf0, 0xd0};;//sizeof
    unsigned char *ciphertext = NULL;
    size_t ciphertext_len;
    unsigned char *decrypted = NULL;
    size_t decrypted_len;
SYSTEM_INFO("plaintext  %d\r\n",sizeof(plaintext));
	SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,(uint8_t *)plaintext,strlen(plaintext),"\r\n");
    // 1. 加密明文
    if (aes_ecb_encrypt(key, 128, (unsigned char *)plaintext, strlen(plaintext),
                       &ciphertext, &ciphertext_len) != 0) {
        SYSTEM_INFO("fail\n");
        return 1;
    }
    print_hex("===e ", ciphertext, ciphertext_len);

    // 2. 解密密文
    if (aes_ecb_decrypt(key, 128, ciphertext, ciphertext_len,
                       &decrypted, &decrypted_len) != 0) {
        SYSTEM_INFO("fail\n");
        free(ciphertext);
        return 1;
    }
    print_hex("===d ", decrypted, decrypted_len);
SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(1,decrypted,decrypted_len,"");
    // 3. 验证解密结果
    if (memcmp(plaintext, decrypted, decrypted_len) == 0) {
        SYSTEM_INFO("success\n");
    } else {
        SYSTEM_INFO("fail\n");
    }

    // 清理内存
    free(ciphertext);
    free(decrypted);
    return 0;
}





SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), aesmain, aesmain, aesmain);

#endif

#include <stdio.h>
#include <string.h>
#include "mbedtls/aes.h"

#define AES_BLOCK_SIZE 16
//success
int test99() {
    // 初始化 AES 上下文
    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);

    // 密钥和明文
    unsigned char key[AES_BLOCK_SIZE] = "0123456789ABCDEF";
    unsigned char input[AES_BLOCK_SIZE] = "1234567890abcdef";
    unsigned char output[AES_BLOCK_SIZE];

    // 设置加密密钥
    if (mbedtls_aes_setkey_enc(&aes, key, AES_BLOCK_SIZE * 8) != 0) {
        SYSTEM_INFO("Failed to set encryption key.\n");
        return 1;
    }

    // 执行 ECB 模式的 AES 加密
    if (mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_ENCRYPT, input, output) != 0) {
        SYSTEM_INFO( "Encryption failed.\n");
        return 1;
    }

    // 输出加密结果
    SYSTEM_INFO("Encrypted value: ");
    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        SYSTEM_INFO("%02x", output[i]);
    }
    SYSTEM_INFO("\n");

    // 清理 AES 上下文
    mbedtls_aes_free(&aes);

    return 0;
}

//SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), test99, test99, test99);


//int test2() {
//    unsigned char key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x25, 0x22, 0x40, 0x9c, 0xf0, 0xd0}; // 示例密钥
//    unsigned char input[] = "要加密的消息"; // 示例输入
//    size_t input_len = strlen((char *)input);
//    unsigned char *output = malloc(((input_len + 15) / 16) * 16); // 输出缓冲区大小根据填充后的输入长度确定

//    int ret = aes_ecb_encrypt(key, 128, input, input_len, output);
//    if (ret == 0) {
//        SYSTEM_INFO("加密成功:\n");
//        for (size_t i = 0; i < ((input_len + 15) / 16) * 16; i++) {
//            SYSTEM_INFO("%02x", output[i]);
//        }
//        SYSTEM_INFO("\n");
//    } else {
//        SYSTEM_INFO("加密失败，错误代码：%d\n", ret);
//    }

//    free(output);
//    return ret;
//}








#if 0


#define AES_KEY_SIZE 16
#define AES_BLOCK_SIZE 16


int aes_encrypted_text(mbedtls_aes_context aes,const unsigned char input[16],unsigned char encrypted_output[16])){

	  
    // 初始化AES上下文
    mbedtls_aes_init(&aes);

    // 设置加密密钥
    mbedtls_aes_setkey_enc(&aes, key, AES_KEY_SIZE * 8);

    // 加密操作
    mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_ENCRYPT, input, encrypted_output);


	SYSTEM_DEBUG_ARRAY_MESSAGE(encrypted,input_size,"\r\n");
	
	
	
	
}




int aes_encrypted_text(mbedtls_aes_context aes,char * input,int input_size,char * encrypted){

	   char* encrypted;
	   encrypted=(char *)malloc(input_size);
	
    // 初始化AES上下文
    mbedtls_aes_init(&aes);

    // 设置加密密钥
    mbedtls_aes_setkey_enc(&aes, key, AES_KEY_SIZE * 8);

    // 加密操作
    mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_ENCRYPT, input, encrypted);


	SYSTEM_DEBUG_ARRAY_MESSAGE(encrypted,input_size,"\r\n");
	
	  mbedtls_aes_free(&aes);

}












#endif

#define AES_KEY_SIZE 16
#define AES_BLOCK_SIZE 16
//success
void test_aes() {
    mbedtls_aes_context aes;
	mbedtls_aes_context aes2;
    unsigned char key[AES_KEY_SIZE] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x25, 0x22, 0x40, 0x9c, 0xf0, 0xd0};
    //unsigned char input[AES_BLOCK_SIZE] = {0x32, 0x43, 0xf6, 0xa8, 0x88, 0x5a, 0x30, 0x8d, 0x31, 0x31, 0x98, 0xa2, 0xe0, 0x37, 0x07, 0x34};
		unsigned char *input = "1234567890abcdef";
		
    unsigned char encrypted[AES_BLOCK_SIZE];
    unsigned char decrypted[AES_BLOCK_SIZE];

    // 初始化AES上下文
    mbedtls_aes_init(&aes);

    // 设置加密密钥
    mbedtls_aes_setkey_enc(&aes, key, AES_KEY_SIZE * 8);

    // 加密操作
    mbedtls_aes_crypt_ecb(&aes, MBEDTLS_AES_ENCRYPT, input, encrypted);

    // 打印加密结果
    SYSTEM_INFO("Encrypted result: ");
    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        SYSTEM_INFO("%02x ", encrypted[i]);
    }
    SYSTEM_INFO("\r\n");

    // 设置解密密钥
    mbedtls_aes_setkey_dec(&aes2, key, AES_KEY_SIZE * 8);

    // 解密操作
    mbedtls_aes_crypt_ecb(&aes2, MBEDTLS_AES_DECRYPT, encrypted, decrypted);

    // 打印解密结果
    SYSTEM_INFO("Decrypted result: ");
    for (int i = 0; i < AES_BLOCK_SIZE; i++) {
        SYSTEM_INFO("%02x ", decrypted[i]);
    }
    SYSTEM_INFO("\r\n");

    // 清理AES上下文
    mbedtls_aes_free(&aes);

   
}

//SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), test_aes, test_aes, test_aes);


#include <stdio.h>
#include <string.h>
#include "mbedtls/cipher.h"

#define AES_KEY_SIZE 16
#define AES_BLOCK_SIZE 16

//success
int test_aes3() {
    size_t ret;
    mbedtls_cipher_context_t cipher_ctx;
    unsigned char key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x25, 0x22, 0x40, 0x9c, 0xf0, 0xd0}; // 示例密钥
    unsigned char iv[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f}; // 示例IV
    unsigned char input[] = "AES-128-CBC-qwertyuiop"; // 示例输入
    int ilen = strlen((char *)input);
    unsigned char encrypted_output[128]; // 加密后的输出缓冲区
    size_t encrypted_olen = 0;
    unsigned char decrypted_output[128]; // 解密后的输出缓冲区
    size_t decrypted_olen = 0;

    // 加密部分（为了得到加密后的密文）
    // 初始化cipher上下文
    mbedtls_cipher_init(&cipher_ctx);

    // 设置cipher类型：AES-128-CBC
    const mbedtls_cipher_info_t *cipher_info = mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_128_CBC);
    if (cipher_info == NULL) {
        SYSTEM_INFO("cipher_info NULL\n");
        mbedtls_cipher_free(&cipher_ctx);
        return -1;
    }

    if ((ret = mbedtls_cipher_setup(&cipher_ctx, cipher_info)) != 0) {
        SYSTEM_INFO("mbedtls_cipher_setup failed\n");
        mbedtls_cipher_free(&cipher_ctx);
        return -1;
    }

    // 设置填充模式为 PKCS#7 填充
    ret = mbedtls_cipher_set_padding_mode(&cipher_ctx, MBEDTLS_PADDING_PKCS7);
    if (ret != 0) {
        SYSTEM_INFO("mbedtls_cipher_set_padding_mode failed\n");
        mbedtls_cipher_free(&cipher_ctx);
        return -1;
    }

    // 设置加密密钥和IV
    if ((ret = mbedtls_cipher_setkey(&cipher_ctx, key, 128, MBEDTLS_ENCRYPT)) != 0) {
        SYSTEM_INFO("mbedtls_cipher_setkey failed\n");
        mbedtls_cipher_free(&cipher_ctx);
        return -1;
    }
    if ((ret = mbedtls_cipher_set_iv(&cipher_ctx, iv, 16)) != 0) {
        SYSTEM_INFO("mbedtls_cipher_set_iv failed\n");
        mbedtls_cipher_free(&cipher_ctx);
        return -1;
    }

    // 更新cipher上下文，准备开始加密
    if ((ret = mbedtls_cipher_reset(&cipher_ctx)) != 0) {
        SYSTEM_INFO("mbedtls_cipher_reset failed\n");
        mbedtls_cipher_free(&cipher_ctx);
        return -1;
    }

    // 执行加密操作
    if ((ret = mbedtls_cipher_update(&cipher_ctx, input, ilen, encrypted_output + encrypted_olen, &encrypted_olen)) != 0) {
        SYSTEM_INFO("mbedtls_cipher_update failed\n");
        mbedtls_cipher_free(&cipher_ctx);
        return -1;
    }
SYSTEM_INFO("encrypted_olen= %d\r\n", encrypted_olen);
    // 完成加密过程
    size_t finish_len = sizeof(encrypted_output) - encrypted_olen; // 剩余可用空间
    if ((ret = mbedtls_cipher_finish(&cipher_ctx, encrypted_output + encrypted_olen, &finish_len)) != 0) {
 
        SYSTEM_INFO("mbedtls_cipher_finish failed with error code %d\n", ret);
        mbedtls_cipher_free(&cipher_ctx);
        return -1;
    }
    encrypted_olen += finish_len;

    // 输出加密后的数据
    SYSTEM_INFO("Encrypted data: ");
    for (int i = 0; i < encrypted_olen; i++) {
        SYSTEM_INFO("%02x", encrypted_output[i]);
    }
    SYSTEM_INFO("\n");

    // 解密部分
    // 重新初始化cipher上下文
    mbedtls_cipher_free(&cipher_ctx);
		
		
		
		
		
		
    mbedtls_cipher_init(&cipher_ctx);

    // 重新设置cipher类型：AES-128-CBC
    cipher_info = mbedtls_cipher_info_from_type(MBEDTLS_CIPHER_AES_128_CBC);
    if (cipher_info == NULL) {
        SYSTEM_INFO("cipher_info NULL\n");
        mbedtls_cipher_free(&cipher_ctx);
        return -1;
    }

    if ((ret = mbedtls_cipher_setup(&cipher_ctx, cipher_info)) != 0) {
        SYSTEM_INFO("mbedtls_cipher_setup failed\n");
        mbedtls_cipher_free(&cipher_ctx);
        return -1;
    }

    // 设置填充模式为 PKCS#7 填充
    ret = mbedtls_cipher_set_padding_mode(&cipher_ctx, MBEDTLS_PADDING_PKCS7);
    if (ret != 0) {
        SYSTEM_INFO("mbedtls_cipher_set_padding_mode failed\n");
        mbedtls_cipher_free(&cipher_ctx);
        return -1;
    }

    // 设置解密密钥和IV
    if ((ret = mbedtls_cipher_setkey(&cipher_ctx, key, 128, MBEDTLS_DECRYPT)) != 0) {
        SYSTEM_INFO("mbedtls_cipher_setkey failed\n");
        mbedtls_cipher_free(&cipher_ctx);
        return -1;
    }
    if ((ret = mbedtls_cipher_set_iv(&cipher_ctx, iv, 16)) != 0) {
        SYSTEM_INFO("mbedtls_cipher_set_iv failed\n");
        mbedtls_cipher_free(&cipher_ctx);
        return -1;
    }

    // 更新cipher上下文，准备开始解密
    if ((ret = mbedtls_cipher_reset(&cipher_ctx)) != 0) {
        SYSTEM_INFO("mbedtls_cipher_reset failed\n");
        mbedtls_cipher_free(&cipher_ctx);
        return -1;
    }

    // 执行解密操作
    if ((ret = mbedtls_cipher_update(&cipher_ctx, encrypted_output, encrypted_olen, decrypted_output + decrypted_olen, &decrypted_olen)) != 0) {
        SYSTEM_INFO("mbedtls_cipher_update failed\n");
        mbedtls_cipher_free(&cipher_ctx);
        return -1;
    }

    // 完成解密过程
    finish_len = sizeof(decrypted_output) - decrypted_olen; // 剩余可用空间
    if ((ret = mbedtls_cipher_finish(&cipher_ctx, decrypted_output + decrypted_olen, &finish_len)) != 0) {

        SYSTEM_INFO("mbedtls_cipher_finish failed with error code %d: \n", ret);
        mbedtls_cipher_free(&cipher_ctx);
        return -1;
    }
    decrypted_olen += finish_len;
SYSTEM_INFO("decrypted_olen= %d\r\n", decrypted_olen);
    // 去除填充
   // decrypted_olen -= decrypted_output[decrypted_olen - 1];
//SYSTEM_INFO("decrypted_olen= %d\r\n", decrypted_olen);
    // 输出解密后的数据
    SYSTEM_INFO("Decrypted data: %.*s\n", (int)decrypted_olen, decrypted_output);

    // 释放cipher上下文
    mbedtls_cipher_free(&cipher_ctx);

    return 0;
}




//SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), test_aes3, test_aes3, test_aes3);








#if 0




#endif





/*********************************************END OF FILE**********************/

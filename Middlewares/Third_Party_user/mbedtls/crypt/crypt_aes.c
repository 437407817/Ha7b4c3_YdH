#include "./crypt/crypt_aes.h" 
#include <stdio.h>
#include "./sys/sysio.h"




//#include "shell.h"





//有这两条语句，会去除编译警告

#define AES_KEY_SIZE     24
#define AES_IV_SIZE     (16+0)										
										
    unsigned char key[AES_KEY_SIZE] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x25, 0x22, 0x40, 0x9c, 0xf0, 0xd0}; // 示例密钥
    unsigned char iv[AES_IV_SIZE] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f}; // 示例IV
//   mbedtls_cipher_type_t cipher_type=MBEDTLS_CIPHER_AES_128_CBC;
		mbedtls_cipher_type_t cipher_type=MBEDTLS_CIPHER_AES_192_CBC;
		//mbedtls_cipher_padding_t padmode=MBEDTLS_PADDING_PKCS7;
		mbedtls_cipher_padding_t padmode=MBEDTLS_PADDING_PKCS7;


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
#if 0
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




//=====================================================================

//success
int testaesmain() {
    const unsigned char key[] = "0123456789ABCDEF"; // 16字节密钥
   // const char *plaintext = "Hello, ECB Mode!Hello, ECB Mode!"; //strlen    // 明文长度16字节（无需填充）
	const char plaintext[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x25, 0x22, 0x40, 0x9c, 0xf0, 0xd0};;//sizeof
    unsigned char *ciphertext = NULL;
    size_t ciphertext_len;
    unsigned char *decrypted = NULL;
    size_t decrypted_len;
//SYSTEM_INFO("plaintext  %d\r\n",sizeof(plaintext));
	SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(0,(uint8_t *)plaintext,strlen(plaintext),"\r\n");
    // 1. 加密明文
    if (aes_ecb_encrypt(key, 128, (unsigned char *)plaintext, strlen(plaintext),
                       &ciphertext, &ciphertext_len) != 0) {
        SYSTEM_INFO("fail\n");
        return 1;
    }
//    SYSTEM_DEBUG_HEX_2(EM_SERIAL_PRINT,"===e ", ciphertext, ciphertext_len);

    // 2. 解密密文
    if (aes_ecb_decrypt(key, 128, ciphertext, ciphertext_len,
                       &decrypted, &decrypted_len) != 0) {
        SYSTEM_INFO("fail\n");
        free(ciphertext);
        return 1;
    }
//    SYSTEM_DEBUG_HEX_2(EM_SERIAL_PRINT,"===d ", decrypted, decrypted_len);
		
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

#endif



//SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), testaesmain, testaesmain, testaesmain);





int aes_ecb_encrypt_CIPHER(mbedtls_cipher_context_t* cipher_ctx,
	                  const unsigned char *key, 
											int key_bitlen,
										const  unsigned char *iv,	
											size_t iv_len,
                    const unsigned char *input, 
											size_t input_len,
										unsigned char* encrypted_output,
                   size_t *output_len,
											const mbedtls_cipher_type_t cipher_type,mbedtls_cipher_padding_t padmode) {
										
		    size_t ret;
   // mbedtls_cipher_context_t cipher_ctx;
    //unsigned char key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x25, 0x22, 0x40, 0x9c, 0xf0, 0xd0}; // 示例密钥
    //unsigned char iv[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f}; // 示例IV
//    unsigned char input[] = "AES-128-CBC-qwertyuiop"; // 示例输入
//    int ilen = strlen((char *)input);
   // unsigned char encrypted_output1[128]; // 加密后的输出缓冲区
    size_t encrypted_olen = 0;
   // unsigned char decrypted_output[128]; // 解密后的输出缓冲区
   // size_t decrypted_olen = 0;

    // 加密部分（为了得到加密后的密文）
    // 初始化cipher上下文
    mbedtls_cipher_init(cipher_ctx);

    // 设置cipher类型：AES-128-CBC
    const mbedtls_cipher_info_t *cipher_info = mbedtls_cipher_info_from_type(cipher_type);//MBEDTLS_CIPHER_AES_128_CBC
    if (cipher_info == NULL) {
        SYSTEM_INFO("cipher_info NULL\n");
        mbedtls_cipher_free(cipher_ctx);
        return 1;
    }

    if ((ret = mbedtls_cipher_setup(cipher_ctx, cipher_info)) != 0) {
        SYSTEM_INFO("mbedtls_cipher_setup failed\n");
        mbedtls_cipher_free(cipher_ctx);
        return 1;
    }

    // 设置填充模式为 PKCS#7 填充
    ret = mbedtls_cipher_set_padding_mode(cipher_ctx, padmode);
    if (ret != 0) {
        SYSTEM_INFO("mbedtls_cipher_set_padding_mode failed\n");
        mbedtls_cipher_free(cipher_ctx);
        return 1;
    }

    // 设置加密密钥和IV
    if ((ret = mbedtls_cipher_setkey(cipher_ctx, key, key_bitlen, MBEDTLS_ENCRYPT)) != 0) {
        SYSTEM_INFO("mbedtls_cipher_setkey failed\n");
        mbedtls_cipher_free(cipher_ctx);
        return 1;
    }
    if ((ret = mbedtls_cipher_set_iv(cipher_ctx, iv, iv_len)) != 0) {
        SYSTEM_INFO("mbedtls_cipher_set_iv failed ,please change 'MBEDTLS_MAX_IV_LENGTH' of file 'cipher.h'\n");
        mbedtls_cipher_free(cipher_ctx);
        return 1;
    }

    // 更新cipher上下文，准备开始加密
    if ((ret = mbedtls_cipher_reset(cipher_ctx)) != 0) {
        SYSTEM_INFO("mbedtls_cipher_reset failed\n");
        mbedtls_cipher_free(cipher_ctx);
        return 1;
    }

    // 执行加密操作
    if ((ret = mbedtls_cipher_update(cipher_ctx, input, input_len, encrypted_output + encrypted_olen, &encrypted_olen)) != 0) {
        SYSTEM_INFO("mbedtls_cipher_update failed\n");
        mbedtls_cipher_free(cipher_ctx);
        return 1;
    }

    // 完成加密过程
    size_t finish_len = 128 - encrypted_olen; // 剩余可用空间
    if ((ret = mbedtls_cipher_finish(cipher_ctx, encrypted_output + encrypted_olen, &finish_len)) != 0) {
 
        SYSTEM_INFO("mbedtls_cipher_finish failed with error code %d\n", ret);
        mbedtls_cipher_free(cipher_ctx);
        return 1;
    }
    encrypted_olen += finish_len;

    // 输出加密后的数据

//		SYSTEM_DEBUG_ARRAY_MESSAGE_HorA(2,encrypted_output,encrypted_olen,"Encrypted data: \r\n");

*output_len=encrypted_olen;
    // 解密部分
    // 重新初始化cipher上下文
    mbedtls_cipher_free(cipher_ctx);								

							return 0;			
										}

int aes_ecb_decrypt_CIPHER(mbedtls_cipher_context_t* cipher_ctx,
	                  const unsigned char *key, 
											int key_bitlen,
										const  unsigned char *iv,
										size_t iv_len,											
                    const unsigned char *encrypted_input, 
										size_t encrypted_ilen,
										unsigned char *decrypted_output,
                   size_t *output_len,
											const mbedtls_cipher_type_t cipher_type,mbedtls_cipher_padding_t padmode) {
	size_t ret;

		size_t decrypted_olen = 0;
		size_t finish_len = 128 - encrypted_ilen; 
    mbedtls_cipher_init(cipher_ctx);

    // 重新设置cipher类型：AES-128-CBC
    
		const mbedtls_cipher_info_t *cipher_info = mbedtls_cipher_info_from_type(cipher_type);
    if (cipher_info == NULL) {
        SYSTEM_INFO("cipher_info NULL\n");
        mbedtls_cipher_free(cipher_ctx);
        return 1;
    }

    if ((ret = mbedtls_cipher_setup(cipher_ctx, cipher_info)) != 0) {
        SYSTEM_INFO("mbedtls_cipher_setup failed\n");
        mbedtls_cipher_free(cipher_ctx);
        return 1;
    }

    // 设置填充模式为 PKCS#7 填充
    ret = mbedtls_cipher_set_padding_mode(cipher_ctx, padmode);
    if (ret != 0) {
        SYSTEM_INFO("mbedtls_cipher_set_padding_mode failed\n");
        mbedtls_cipher_free(cipher_ctx);
        return 1;
    }

    // 设置解密密钥和IV
    if ((ret = mbedtls_cipher_setkey(cipher_ctx, key, key_bitlen, MBEDTLS_DECRYPT)) != 0) {
        SYSTEM_INFO("mbedtls_cipher_setkey failed\n");
        mbedtls_cipher_free(cipher_ctx);
        return 1;
    }
    if ((ret = mbedtls_cipher_set_iv(cipher_ctx, iv, iv_len)) != 0) {
        SYSTEM_INFO("mbedtls_cipher_set_iv failed\n");
        mbedtls_cipher_free(cipher_ctx);
        return 1;
    }

    // 更新cipher上下文，准备开始解密
    if ((ret = mbedtls_cipher_reset(cipher_ctx)) != 0) {
        SYSTEM_INFO("mbedtls_cipher_reset failed\n");
        mbedtls_cipher_free(cipher_ctx);
        return 1;
    }

    // 执行解密操作
    if ((ret = mbedtls_cipher_update(cipher_ctx, encrypted_input, encrypted_ilen, decrypted_output + decrypted_olen, &decrypted_olen)) != 0) {
        SYSTEM_INFO("mbedtls_cipher_update failed\n");
        mbedtls_cipher_free(cipher_ctx);
        return 1;
    }

    // 完成解密过程
    finish_len = 128 - decrypted_olen; // 剩余可用空间
    if ((ret = mbedtls_cipher_finish(cipher_ctx, decrypted_output + decrypted_olen, &finish_len)) != 0) {

        SYSTEM_INFO("mbedtls_cipher_finish failed with error code %d: \n", ret);
        mbedtls_cipher_free(cipher_ctx);
        return 1;
    }
    decrypted_olen += finish_len;
//SYSTEM_INFO("decrypted_olen= %d\r\n", decrypted_olen);

    // 输出解密后的数据
    //SYSTEM_INFO("Decrypted data: %.*s\n", (int)decrypted_olen, decrypted_output);

    // 释放cipher上下文
    mbedtls_cipher_free(cipher_ctx);

*output_len=decrypted_olen;
							return 0;				
}										
										
										
//=====================================================================

										


int testencrypt16() {
    mbedtls_cipher_context_t cipher_ctx;
	mbedtls_cipher_context_t cipher_ctx2;
    unsigned char key[AES_KEY_SIZE] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x25, 0x22, 0x40, 0x9c, 0xf0, 0xd0}; // 示例密钥
    unsigned char iv[AES_IV_SIZE] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f}; // 示例IV
    unsigned char input[] = "AES-128-CBC-qwertyuiopabcdefghiqwertyuiop"; // 示例输入
		
		
    int ilen = strlen((char *)input);
unsigned char encrypted_output[128];
		size_t encrypted_olen = 0;
		unsigned char decrypted_output[128]; // 解密后的输出缓冲区
		size_t decrypted_olen = 0;
   //mbedtls_cipher_type_t cipher_type=MBEDTLS_CIPHER_AES_128_CBC;
		mbedtls_cipher_type_t cipher_type=MBEDTLS_CIPHER_AES_192_CBC;
		//mbedtls_cipher_padding_t padmode=MBEDTLS_PADDING_PKCS7;
		mbedtls_cipher_padding_t padmode=MBEDTLS_PADDING_PKCS7;
	aes_ecb_encrypt_CIPHER(&cipher_ctx,key,sizeof(key)*8,iv,sizeof(iv),input,ilen,encrypted_output,&encrypted_olen,cipher_type,padmode);
	//SYSTEM_DEBUG_ARRAY_MESSAGE_2(EM_SERIAL_PRINT,encrypted_output,encrypted_olen,"outertest==");
	
	SYSTEM_DEBUG_ARRAY_MESSAGE(encrypted_output,encrypted_olen,"outertest==");
		
		aes_ecb_decrypt_CIPHER(&cipher_ctx2,key,sizeof(key)*8,iv,sizeof(iv),encrypted_output,encrypted_olen,decrypted_output,&decrypted_olen,cipher_type,padmode);
		
		printf("Decrypted data: %.*s\r\n", (int)decrypted_olen, decrypted_output);
		
		//SEGGER_RTT_printf(0,"Decrypted data: %-30.*s\r\n", (int)decrypted_olen, decrypted_output);
		SYSTEM_DEBUG_STRING_2(EM_SEGGER_PRINT,decrypted_output,decrypted_olen);
		SYSTEM_DEBUG_STRING_2(EM_SERIAL_PRINT,decrypted_output,decrypted_olen);
}




//SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC), testencrypt16, testencrypt16, testencrypt16);

int mbedtls_aes_ecb_encrypt16(unsigned char *input,uint16_t inputlength,unsigned char *encrypted_output,size_t *encrypted_olen){
 mbedtls_cipher_context_t cipher_ctx;

//unsigned char encrypted_output[128];


//		mbedtls_cipher_type_t cipher_type=MBEDTLS_CIPHER_AES_192_CBC;
//		
//		mbedtls_cipher_padding_t padmode=MBEDTLS_PADDING_PKCS7;
	return aes_ecb_encrypt_CIPHER(&cipher_ctx,key,sizeof(key)*8,iv,sizeof(iv),input,inputlength,encrypted_output,encrypted_olen,cipher_type,padmode);
	//SYSTEM_DEBUG_ARRAY_MESSAGE_2(EM_SERIAL_PRINT,encrypted_output,encrypted_olen,"outertest==");
	
//	SYSTEM_DEBUG_ARRAY_MESSAGE(encrypted_output,*encrypted_olen,"outertest==");

}


int mbedtls_aes_ecb_dencrypt16(unsigned char *encrypted_input,uint16_t inputlength,unsigned char *decrypted_output,size_t *decrypted_olen){
 	mbedtls_cipher_context_t cipher_ctx2;
	
	
	
//		unsigned char decrypted_output[128]; // 解密后的输出缓冲区
//		size_t decrypted_olen = 0;

	
	
		return	aes_ecb_decrypt_CIPHER(&cipher_ctx2,key,sizeof(key)*8,iv,sizeof(iv),encrypted_input,inputlength,decrypted_output,decrypted_olen,cipher_type,padmode);
//		SYSTEM_INFO("===%d\r\n", i);
//		SYSTEM_INFO("Decrypted data: %.*s\r\n", (int)dencrypted_olen, dencrypted_output);
		
		//SEGGER_RTT_SYSTEM_INFO(0,"Decrypted data: %-30.*s\r\n", (int)decrypted_olen, decrypted_output);
//		SYSTEM_DEBUG_STRING(decrypted_output,decrypted_olen);
//SYSTEM_DEBUG_STRING_2(EM_SEGGER_PRINT,decrypted_output,*decrypted_olen);
	
	
	
	
	

}





/*********************************************END OF FILE**********************/

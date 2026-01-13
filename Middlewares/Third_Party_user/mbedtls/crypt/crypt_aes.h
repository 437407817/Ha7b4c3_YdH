#ifndef __CRYPT_AES_H
#define	__CRYPT_AES_H


#include "stm32f4xx.h"

#include <string.h>
#include <stdint.h>

#include "mbedtls/aes.h"
#include "mbedtls/cipher.h"


// AES-ECB º”√‹
int aes_ecb_encrypt(const unsigned char *key, size_t key_bits,
                    const unsigned char *input, size_t input_len,
                    unsigned char **output, size_t *output_len) ;


// AES-ECB Ω‚√‹
int aes_ecb_decrypt(const unsigned char *key, size_t key_bits,
                    const unsigned char *input, size_t input_len,
                    unsigned char **output, size_t *output_len);



int aes_ecb_encrypt_CIPHER(mbedtls_cipher_context_t* cipher_ctx,
	                  const unsigned char *key, 
											int key_bitlen,
										const  unsigned char *iv,	
											size_t iv_len,
                    const unsigned char *input, 
											size_t input_len,
										unsigned char* encrypted_output,
                   size_t *output_len,
											const mbedtls_cipher_type_t cipher_type,mbedtls_cipher_padding_t padmode);

										
										
										
int aes_ecb_decrypt_CIPHER(mbedtls_cipher_context_t* cipher_ctx,
	                  const unsigned char *key, 
											int key_bitlen,
										const  unsigned char *iv,
										size_t iv_len,											
                    const unsigned char *encrypted_input, 
										size_t encrypted_ilen,
										unsigned char *decrypted_output,
                   size_t *output_len,
											const mbedtls_cipher_type_t cipher_type,mbedtls_cipher_padding_t padmode);										
										
										
										
										
int testencrypt16();										
										
int mbedtls_aes_ecb_encrypt16(unsigned char *input,uint16_t inputlength,unsigned char *encrypted_output,size_t *encrypted_olen);										
int mbedtls_aes_ecb_dencrypt16(unsigned char *encrypted_input,uint16_t inputlength,unsigned char *dencrypted_output,size_t *dencrypted_olen);										

#endif /* __CRYPT_AES_TEST_H */

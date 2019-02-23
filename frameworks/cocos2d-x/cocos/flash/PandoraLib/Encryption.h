//
//  Encryption.h
//  DownloadFile
//
//  Created by zhoumin on 12-1-16.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@class NSString;

@interface NSData (Encryption)

- (NSData *)AES128EncryptWithKey:(NSString *)key iv:(NSString*)iv;   //加密
- (NSData *)AES128DecryptWithKey:(NSString *)key iv:(NSString*)iv;   //解密
- (NSString *)newStringInBase64FromData;            //追加64编码
+ (NSString*)base64encode:(NSString*)str;           //同上64编码

@end

//
//  UartViewController.h
//  WS BLE
//
//  Created by 杨伟标 on 16/9/7.
//  Copyright © 2016年 waveshare. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "SerialGATT.h"

@interface UartViewController : UIViewController<BTSmartSensorDelegate,UITextFieldDelegate>

@property (strong, nonatomic) SerialGATT *sensor;
@property (weak, nonatomic) IBOutlet UITextField *SendDataTextField;
@property (weak, nonatomic) IBOutlet UITextView *ReceiveDataTextView;
@property (weak, nonatomic) IBOutlet UIButton *SendButton;
@property (weak, nonatomic) IBOutlet UILabel *SendCountLabel;
@property (weak, nonatomic) IBOutlet UILabel *ReceivedCountLabel;

@property int SendDataCount;
@property int ReceivedDataCount;

@property NSString *SendCountLabelString;
@property NSString *ReceivedCountLabelString;

@end

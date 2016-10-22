//
//  ScanTableTableViewController.h
//  WS BLE
//
//  Created by 杨伟标 on 16/9/5.
//  Copyright © 2016年 waveshare. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "ViewController.h"
#import "SerialGATT.h"

@class ViewController;

@interface ScanTableTableViewController : UITableViewController<BTSmartSensorDelegate>

@property (weak, nonatomic) IBOutlet UIBarButtonItem *scanButton;

@property (nonatomic, retain) NSMutableArray *peripheralArray;

@property (strong, nonatomic) SerialGATT *sensor;

@end

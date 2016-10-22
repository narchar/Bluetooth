//
//  ViewController.m
//  WS BLE
//
//  Created by 杨伟标 on 16/9/5.
//  Copyright © 2016年 waveshare. All rights reserved.
//

#import "ViewController.h"

@interface ViewController ()

@end

@implementation ViewController

@synthesize peripheral;
@synthesize sensor;
@synthesize UartButton;
@synthesize RomoteControlButton;
@synthesize DeviceControlButton;
@synthesize Label1;
@synthesize Label2;

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.

    self.sensor.delegate = self;
    
    UIBarButtonItem *leftBarButtonItem = [[UIBarButtonItem alloc]
                                          initWithTitle:NSLocalizedString(@"Button_Back", @"")
                                          style:UIBarButtonItemStylePlain
                                          target:self
                                          action:@selector(doClickBackAction)
                                          ];
    
    self.navigationItem.leftBarButtonItem = leftBarButtonItem;
    
    [Label1 setText:sensor.activePeripheral.name];

    [Label2 setText:[NSString stringWithFormat:@"%@：%@",@"UUID",sensor.activePeripheral.identifier.UUIDString]];
    
    [UartButton setTitle:NSLocalizedString(@"Button_Uart", @"") forState:UIControlStateNormal];
    
    [RomoteControlButton setTitle:NSLocalizedString(@"Button_Romote_Control", @"") forState:UIControlStateNormal];
    
    [DeviceControlButton setTitle:NSLocalizedString(@"Button_Device_Control", @"") forState:UIControlStateNormal];
}

- (void)doClickBackAction
{
    if (sensor.activePeripheral.state == CBPeripheralStateConnected)
    {
        [sensor disconnect:sensor.activePeripheral];
    }
    self.sensor.delegate = nil;
    [self.navigationController popViewControllerAnimated:YES];
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
- (IBAction)RomoteButtonClick:(UIButton *)sender {
    //将我们的storyBoard实例化，“Main”为StoryBoard的名称
    UIStoryboard *mainStoryBoard = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
    
    //将第二个控制器实例化，"*Controller"为我们设置的控制器的ID
    ViewController *peripheralController = [mainStoryBoard instantiateViewControllerWithIdentifier:@"romoteController"];
    
    peripheralController.sensor = self.sensor;
    
    //跳转事件
    [self.navigationController pushViewController:peripheralController animated:YES];
}

- (IBAction)UartButtonClick:(id)sender {
    UIStoryboard *mainStoryBoard = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
    
    ViewController *peripheralController = [mainStoryBoard instantiateViewControllerWithIdentifier:@"uartController"];
    
    peripheralController.sensor = self.sensor;
    
    [self.navigationController pushViewController:peripheralController animated:YES];
}

- (IBAction)DeviceButton_Click:(UIButton *)sender {
    UIStoryboard *mainStoryBoard = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
    
    ViewController *peripheralController = [mainStoryBoard instantiateViewControllerWithIdentifier:@"deviceController"];
    
    peripheralController.sensor = self.sensor;
    
    [self.navigationController pushViewController:peripheralController animated:YES];
}


#pragma mark - BleDelegate
-(void) peripheralFound:(CBPeripheral *)peripheral
{
}

//recv data
-(void) serialGATTCharValueUpdated:(NSString *)UUID value:(NSData *)data
{
}

-(void)setConnect
{
}

-(void)setDisconnect
{
    [sensor disconnect:sensor.activePeripheral];
    self.sensor.delegate = nil;
    [self.navigationController popViewControllerAnimated:YES];
    
    UIAlertView * alert = [[UIAlertView alloc]initWithTitle:
                        NSLocalizedString(@"Connection_Alert_Title", @"")
                        message:NSLocalizedString(@"Connection_Alert_Content", @"")
                        delegate:nil
                        cancelButtonTitle:NSLocalizedString(@"Connection_Alert_Button", @"")
                        otherButtonTitles:nil];
    
    [alert show];
}

@end

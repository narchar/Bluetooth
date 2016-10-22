//
//  UartViewController.m
//  WS BLE
//
//  Created by 杨伟标 on 16/9/7.
//  Copyright © 2016年 waveshare. All rights reserved.
//

#import "UartViewController.h"

@interface UartViewController ()

@end

@implementation UartViewController

@synthesize sensor;
@synthesize SendDataTextField;
@synthesize ReceiveDataTextView;
@synthesize SendButton;
@synthesize SendCountLabel;
@synthesize ReceivedCountLabel;
@synthesize SendDataCount;
@synthesize ReceivedDataCount;
@synthesize SendCountLabelString;
@synthesize ReceivedCountLabelString;

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.sensor.delegate = self;
    
    UIBarButtonItem *leftBarButtonItem = [[UIBarButtonItem alloc]
                                          initWithTitle:NSLocalizedString(@"Button_Back", @"")
                                          style:UIBarButtonItemStylePlain
                                          target:self
                                          action:@selector(doClickBackAction)
                                          ];
    
    self.navigationItem.leftBarButtonItem = leftBarButtonItem;
    
    SendDataTextField.delegate = self;
    
    [SendButton setTitle:NSLocalizedString(@"Button_Send", @"") forState:UIControlStateNormal];
    SendButton.enabled = false;
    
    [NSTimer scheduledTimerWithTimeInterval:0.2 target:self selector:@selector(delayMethod) userInfo:nil repeats:NO];
    
    SendCountLabelString = NSLocalizedString(@"Label_Send", @"");
    ReceivedCountLabelString = NSLocalizedString(@"Label_Received", @"");

    SendDataCount = 0;
    SendCountLabel.text = [NSString stringWithFormat:@"%@：%d",SendCountLabelString,SendDataCount];

    ReceivedDataCount = 0;
    ReceivedCountLabel.text = [NSString stringWithFormat:@"%@：%d",ReceivedCountLabelString,ReceivedDataCount];
    
    //增加监听，当键盘出现或改变时收出消息
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(keyboardWillShow:)
                                                 name:UIKeyboardWillShowNotification
                                               object:nil];

}

//当键盘出现或改变时调用
- (void)keyboardWillShow:(NSNotification *)aNotification
{
    //获取键盘的高度
    NSDictionary *userInfo = [aNotification userInfo];
    NSValue *aValue = [userInfo objectForKey:UIKeyboardFrameEndUserInfoKey];
    CGRect keyboardRect = [aValue CGRectValue];
    int height = keyboardRect.size.height;
  
    CGRect frame = SendDataTextField.frame;
  
    int offset = frame.origin.y + SendDataTextField.frame.size.height - (self.view.frame.size.height - height);//键盘高度216
    
    NSTimeInterval animationDuration = 0.30f;
    [UIView beginAnimations:@"ResizeForKeyboard" context:nil];
    [UIView setAnimationDuration:animationDuration];
    
    //将视图的Y坐标向上移动offset个单位，以使下面腾出地方用于软键盘的显示
    if(offset > 0)
        self.view.frame = CGRectMake(0.0f, -offset, self.view.frame.size.width, self.view.frame.size.height);
    
    [UIView commitAnimations];
}

- (void)delayMethod
{
    [sensor connect:sensor.activePeripheral];
}

- (void)doClickClearAction
{
    ReceiveDataTextView.text = @"";
    
    SendDataCount = 0;
    SendCountLabel.text = [NSString stringWithFormat:@"%@：%d",SendCountLabelString,SendDataCount];
    
    ReceivedDataCount = 0;
    ReceivedCountLabel.text = [NSString stringWithFormat:@"%@：%d",ReceivedCountLabelString,ReceivedDataCount];
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

//当用户按下return键或者按回车键，keyboard消失
-(BOOL)textFieldShouldReturn:(UITextField *)textField
{
    [textField resignFirstResponder];
    return YES;
}

//输入框编辑完成以后，将视图恢复到原始状态
-(void)textFieldDidEndEditing:(UITextField *)textField
{
    self.view.frame =CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height);
}


- (IBAction)SendButtonClick:(id)sender {
    NSString *str = SendDataTextField.text;
    NSData *data = [str dataUsingEncoding:NSUTF8StringEncoding];
    [sensor write:sensor.activePeripheral data:data];
    SendDataCount += [str length];
    SendCountLabel.text = [NSString stringWithFormat:@"%@：%d",SendCountLabelString,SendDataCount];
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

/*
#pragma mark - Navigation

// In a storyboard-based application, you will often want to do a little preparation before navigation
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {
    // Get the new view controller using [segue destinationViewController].
    // Pass the selected object to the new view controller.
}
*/


#pragma mark - BleDelegate
-(void) peripheralFound:(CBPeripheral *)peripheral
{
}

//recv data
-(void) serialGATTCharValueUpdated:(NSString *)UUID value:(NSData *)data
{
    NSString *value = [[NSString alloc] initWithData:data encoding:NSASCIIStringEncoding];
    
    if (ReceiveDataTextView.text.length >= 4096)
    {
        ReceiveDataTextView.text = @"";
    }
    
    ReceivedDataCount += [value length];
    ReceivedCountLabel.text = [NSString stringWithFormat:@"%@：%d",ReceivedCountLabelString,ReceivedDataCount];
    
    ReceiveDataTextView.text = [ReceiveDataTextView.text stringByAppendingString:value];
    
    [ReceiveDataTextView scrollRectToVisible:CGRectMake(0, ReceiveDataTextView.contentSize.height-15, ReceiveDataTextView.contentSize.width, 10) animated:YES];
}

-(void)setConnect
{
    UIBarButtonItem *rightBarButtonItem = [[UIBarButtonItem alloc]
                                           initWithTitle:NSLocalizedString(@"Button_Clear", @"")
                                           style:UIBarButtonItemStylePlain
                                           target:self
                                           action:@selector(doClickClearAction)
                                           ];
    
    self.navigationItem.rightBarButtonItem = rightBarButtonItem;
    
    SendDataTextField.text = @"Waveshare";

    SendButton.enabled = true;
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

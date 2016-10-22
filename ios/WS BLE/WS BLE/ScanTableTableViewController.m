//
//  ScanTableTableViewController.m
//  WS BLE
//
//  Created by 杨伟标 on 16/9/5.
//  Copyright © 2016年 waveshare. All rights reserved.
//

#import "ScanTableTableViewController.h"

@interface ScanTableTableViewController ()

@end

@implementation ScanTableTableViewController

@synthesize scanButton;
@synthesize peripheralArray;
@synthesize sensor;

- (void)viewDidLoad {
    [super viewDidLoad];
    
    sensor = [[SerialGATT alloc] init];
    [sensor setup];
    
    peripheralArray = [[NSMutableArray alloc] init];
    
    //设置Scan
    [scanButton setTitle:NSLocalizedString(@"Button_Scan", @"")];
}

- (void)viewWillAppear:(BOOL)animated
{
    [super viewWillAppear:YES];
    
    sensor.delegate = self;
    
    //和扫描按键函数相同
    [sensor.peripherals removeAllObjects];//清空SerialGATT内部的数据
    [peripheralArray removeAllObjects];
    [self.tableView reloadData];
}


- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (IBAction)scanButtonClick:(id)sender {
    [scanButton setEnabled:NO];//防止扫描速度太快
    [scanButton setTitle:@"... ..."];
    
    [sensor.peripherals removeAllObjects];//清空SerialGATT内部的数据
    [peripheralArray removeAllObjects];
    [self.tableView reloadData];
    [sensor findBlePeripherals:2];
    [NSTimer scheduledTimerWithTimeInterval:2 target:self selector:@selector(scanTimer:) userInfo:nil repeats:NO];
}

-(void) scanTimer:(NSTimer *)timer
{
    [scanButton setTitle:NSLocalizedString(@"Button_Scan", @"")];
    
    [sensor stopScan];
    [scanButton setEnabled:YES];
}

#pragma mark - Table view data source

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
  return [self.peripheralArray count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    static NSString *cellId = @"peripheral";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cellId];
    if (cell == nil) {
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellId];
    }
    
    CBPeripheral *peripheralTemp = [peripheralArray objectAtIndex:[indexPath row]];
    
    cell.textLabel.text = peripheralTemp.name;
    cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
    
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    CBPeripheral *peripheralTemp = [peripheralArray objectAtIndex:[indexPath row]];
    
    sensor.activePeripheral = peripheralTemp;
    
    //将我们的storyBoard实例化，“Main”为StoryBoard的名称
    UIStoryboard *mainStoryBoard = [UIStoryboard storyboardWithName:@"Main" bundle:nil];
    
    //将第二个控制器实例化，"*Controller"为我们设置的控制器的ID
    ViewController *peripheralController = [mainStoryBoard instantiateViewControllerWithIdentifier:@"peripheralController"];
    
    peripheralController.sensor = self.sensor;
    
    //跳转事件
    [self.navigationController pushViewController:peripheralController animated:YES];

}

/*
// Override to support conditional editing of the table view.
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath {
    // Return NO if you do not want the specified item to be editable.
    return YES;
}
*/

/*
// Override to support editing the table view.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath {
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        // Delete the row from the data source
        [tableView deleteRowsAtIndexPaths:@[indexPath] withRowAnimation:UITableViewRowAnimationFade];
    } else if (editingStyle == UITableViewCellEditingStyleInsert) {
        // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
    }   
}
*/

/*
// Override to support rearranging the table view.
- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath {
}
*/

/*
// Override to support conditional rearranging of the table view.
- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath {
    // Return NO if you do not want the item to be re-orderable.
    return YES;
}
*/

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
    CBPeripheral *peripheralTemp = peripheral;
    [peripheralArray addObject:peripheralTemp];
    [self.tableView reloadData];
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
    UIAlertView * alert = [[UIAlertView alloc]initWithTitle:
            NSLocalizedString(@"Connection_Alert_Title", @"")
            message:NSLocalizedString(@"Connection_Alert_Content", @"")
            delegate:nil
            cancelButtonTitle:NSLocalizedString(@"Connection_Alert_Button", @"")
            otherButtonTitles:nil];
    
    [alert show];
    
    //和扫描按键函数相同
    [scanButton setEnabled:NO];//防止扫描速度太快
    [sensor.peripherals removeAllObjects];//清空SerialGATT内部的数据
    [peripheralArray removeAllObjects];
    [self.tableView reloadData];
    [sensor findBlePeripherals:2];
    [scanButton setTitle:@"... ..."];
    [NSTimer scheduledTimerWithTimeInterval:2 target:self selector:@selector(scanTimer:) userInfo:nil repeats:NO];
}

@end

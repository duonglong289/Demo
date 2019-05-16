## Mx12() ##

Constructor này mở cổng giao tiếp UART từ Raspberry Pi tới Dynamixel với baudrate = 1000000. 

wiringPiSetupGpio();	// *Thiết lập kiểu đánh số chân theo Broadcom GPIO*

Thiết lập chân điều khiển tín hiệu vào ra.


## readData(int id) ##

Nhận gói Status Packet sau đó xác định các giá trị trả về tương ứng với các Instruction Packet

## ping(int id) ##

Kiểm tra kết nối tới Dynamixel

## factoryReset (int id, bool confirm) ##

Thiết lập lại Control Table, tất cả các giá trị thanh ghi sẽ được đặt lại về trạng thái mặc định ban đầu

## setID (int id, int newID) ##

Đặt lại ID cho Dynamixel

## setBaudRate(int id, int baudRate) ##

Thiết lập Baud rate cho giao tiếp giữa Raspberry Pi và Dynamixel

## setStatusReturnLevel (int id, int Level)







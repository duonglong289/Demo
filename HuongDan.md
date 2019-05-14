**1. Cài đặt thư viện trên Raspberry Pi:**

Bước 1: Kiểm tra thư viện đã cài đặt hay chưa. Mở Terminal gõ lệnh :

	$ gpio -v

Nếu thư viện đã được cài đặt sẽ được hiện thông báo. Nếu chưa cài đặt, thực hiện các bước tiếp theo.

Bước 2: Download bộ thư viện và các ví dụ về WiringPi về kit, trên Terminal :

	$ git clone git://git.drogon.net/wiringPi

Sau khi download, thư viện wiringPi sẽ nằm trong thư mục /home/Pi

Bước 3: Chuyển đến thư mục wiringPi để tiến hành build thư viện :

	$ cd wiringPi

	$ ./build

Bước 4: Kiểm tra lại thư viện đã được cài đặt thành công hay không :

	$ gpio -v
	
	$ gpio readall

Note : Thư viện WiringSerial.h cũng nằm trong thư mục wiringPi

**2. Cấu hình giao tiếp Dynamixel với Raspberry Pi:**
- Giao tiếp giữa Raspberry Pi và Dynamixel thông qua cổng UART, nhưng chỉ có một dây tín hiệu cho cả đường truyền và nhận.
Để thực hiện điều này cần xây dựng một mạch nhỏ chuyển đổi song công hoàn toàn thành bán song công.
- Sử dụng IC 74HC126 và 74HC125 (hoặc 74HC04)
<img src="https://i.imgur.com/7RktWcv.jpg">

[Huongdan](https://github.com/duonglong289/Demo/blob/master/HuongDan.md)

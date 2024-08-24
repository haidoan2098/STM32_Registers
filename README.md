# Lesson 1: Boot Process Of ARM Cortex-M Core MCU. 

## A. REGISTER & MEMORY MAP.

### I. Các chế độ hoạt động.
**1.** ***Thread Mode:*** Đây là chế độ bình thường của chương trình. Tất cả code chương trình trong main() sẽ được thực thi ở chế độ này.      
**2.** ***Handler Mode:*** Tất cả các xử lý System Exception (Xử lý ngoại lệ) và Interrupt (Ngắt) sẽ được thực thi dưới chế độ Handler mode. Bất cứ khi nào vi xử lý gặp System Exception hoặc Interrupt đến từ bất cứ ngoại vi nào, thì vi xử lý lập tức chuyển từ Thread mode -> Handler mode và thực thi, sau khi xử lý xong thì sẽ quay ở lại Thread mode.


### II. Registers of MPU.
![](https://khuenguyencreator.com/wp-content/uploads/2021/07/Core-Register-768x565.png)

**Các thanh ghi quan trọng**
- **Thanh ghi R13 được gọi là Stack Pointer (SP).** Thanh ghi này được sử dụng để theo dõi bộ nhớ Stack. Bên cạnh thanh ghi R13, có thêm 2 thanh ghi nữa là PSP (Processor Stack Pointer) và MSP (Main Stack Pointer) những thanh ghi này được gọi là Banked version of SP.     
- **Thanh ghi R14 được gọi là Link Register (LR).** Nó lưu thông tin của subroutines (chương trình con), function call và exceptions.       
- **Thanh ghi R15 được gọi là Program Counter (PC).** Thanh ghi này chứa địa chỉ câu lệnh sẽ được thực thi. Khi Reset, vi xử lý sẽ nạp thanh ghi PC với giá trị reset là 0x0000_0000. Sau đó, nó sẽ đọc giá trị tại địa chỉ 0x0000_0004 để xác định điểm bắt đầu thực thi chương trình.


### III. Memory Map của STM32.
- STM32 là Vi Điều Khiển dòng ARM lõi Cortex-M.
- Tất cả các Vi Xử Lý ARM Coxtex-M đều có 32-bit địa chỉ bộ nhớ, điều này có nghĩa là không gian bộ nhớ tối đa có thể lên đến 4GB.

![](https://khuenguyencreator.com/wp-content/uploads/2021/07/memory-map-768x607.png)


## B. BOOT PROCESS & MEMORY PARTITIONING.

### I. Boot Process.

**Trình tự khởi động như sau:**  

**1. Khi có sự kiện Reset:**
- Quá trình khởi động bắt đầu khi có sự kiện Reset (có thể do nhấn nút reset, bật nguồn, watchdog timeout, v.v.).   

**2. Xác định chế độ khởi động:**       
- Vi điều khiển kiểm tra trạng thái chân BOOT để xác định chế độ khởi động (ví dụ: boot từ Flash, bootloader, hoặc System Memory).   

**3. Khởi tạo Main Stack Pointer (MSP):**       
- Vi điều khiển đọc giá trị tại địa chỉ 0x0000_0000 từ Vector Table. Giá trị này được sử dụng để thiết lập Main Stack Pointer (MSP), tức là địa chỉ bắt đầu của vùng nhớ Stack.
        
**4. Khởi tạo Program Counter (PC):**
- Tại địa chỉ 0x0000_0004, vi điều khiển đọc giá trị để khởi tạo thanh ghi Program Counter (PC). PC sẽ nhảy đến địa chỉ này và thực thi mã lệnh tại đó, thường là hàm Reset_Handler.

**5. Thực thi Reset_Handler:**
- Reset_Handler là hàm đầu tiên được thực thi sau khi Reset và đảm nhiệm các bước khởi tạo cần thiết:       
-- Khởi tạo hệ thống Clock: Cấu hình và bật các nguồn clock.        
-- Khởi tạo các thanh ghi quan trọng.       
-- Khởi tạo vùng nhớ .bss, .data.  
-- Copy dữ liệu từ Flash sang RAM cho vùng .bss, .data: Sao chép dữ liệu khởi tạo từ Flash sang RAM cho các biến toàn cục đã được khởi tạo.     

**6. Gọi hàm main():**
- Sau khi hoàn thành các bước khởi tạo trong Reset_Handler, vi điều khiển sẽ gọi hàm main() để bắt đầu chương trình chính.  

**7. Thực thi vòng lặp chính:**
- Trong hàm main(), thường có một vòng lặp vô hạn (while(1)) để thực hiện các chức năng chính của chương trình.

![](https://i.imgur.com/VnZWlrr.jpeg)

***Cụ thể trong phân vùng Code***

<img src="https://khuenguyencreator.com/wp-content/uploads/2021/07/code-memory-768x578.png" width="400" height="300" alt="Cụ thể trong phân vùng Code">


**a. *Ví dụ quá trình khởi động:***     

![](https://i.imgur.com/J5W0rFv.jpeg)

- Đọc giá trị tại địa chỉ 0x0000_0000 nạp cho thanh ghi MSP. (Thực chất MSP này sẽ trỏ đến địa chỉ của chương trình trong SRAM (do nó trỏ đến địa chỉ > 0x2000_0000)). Ở giai đoạn này, MSP đang ánh xạ đến địa chỉ 0x0800_0000 - địa chỉ đầu tiên của vùng nhớ Flash và có giá trị vừa gán.
- Đọc giá trị tại địa chỉ 0x0000_0004 nạp cho thanh ghi PC. Thanh ghi PC sẽ vào địa chỉ đó và thực hiện Reset_Handler là điểm vào đầu tiên khi VĐK khởi động.       
-- PC sau đó nhảy vào Stack là nơi thực hiện các hàm lệnh trong main().

**b. *Các chế độ khởi động:***

<img src="https://i.imgur.com/lKqjFIq.png" width="500" height="300" alt="Cụ thể trong phân vùng Code">      

**1. Boot từ Bộ Nhớ Flash Chính (Main Flash Memory)** 

<img src="https://i.imgur.com/FKZLb5a.jpeg" width="400" height="300">

- Mã khởi động nằm trong bộ nhớ flash và thực thi từ địa chỉ 0x0800_0000.
- Bộ vi xử lý sẽ ánh xạ bộ nhớ flash từ bên trong, nghĩa là ta có thể truy cập bộ nhớ flash thông qua địa chỉ 0x0800_0000 hoặc là 0x0000_0000.

**2. Boot từ Bộ Nhớ Hệ Thống (System Memory)** 

<img src="https://i.imgur.com/jaMzBF7.png" width="400" height="300">

- Mã khởi động nằm trong bộ nhớ hệ thống (system memory) và thực thi từ địa chỉ 0x1FFF_0000.
- Bộ vi xử lý sẽ ánh xạ bộ nhớ flash từ bên trong, nghĩa là ta có thể truy cập bộ nhớ flash thông qua địa chỉ 0x0800_0000 hoặc là 0x1FFF_0000.
- Ở chế độ này, bộ vi xử lý có thể lập trình lại bộ nhớ Flash hoặc sử dụng cho các chức năng đặc biệt như nâng cấp firmware.

**3. Boot từ SRAM** (Này ít dùng nên sẽ ko nói tới).


### II. Memory Partitioning - Phân vùng bộ nhớ.

**1. Phân vùng bộ nhớ Flash - RAM**  

**a. Phân vùng bộ nhớ Flash.**   

<img src="https://i.imgur.com/7co6ayp.png" width="400" height="350">          

- **.isr_vector**: Lưu Vector Table (hay Vector ngắt).      
- **.text**: Lưu các lệnh, hàm và các chỉ thị lệnh.     
- **.rodata**: Lưu các biến dạng Const.     
- **.data**: Lưu các biến toàn cục và Static.       

**b. Phân vùng bộ nhớ RAM.**     

<img src="https://i.imgur.com/53Oug6B.png" width="400" height="400">  

- **.data**: Lưu biến toàn cục hoặc biến Static đã có giá trị ban đầu.      
- **.bss**: Lưu biến toàn cục hoặc biến Static chưa có giá trị ban đầu.     
- **.stack**: Dùng để lưu trữ địa chỉ của hàm được gọi và biến cục bộ của hàm. Hoạt động theo cơ chế LIFO.      
- **.heap**: Các biến được cấp phát động (Calloc - Malloc - Free).      

***Thực thi Reset_Handler, copy dữ liệu từ Flash -> RAM***
<img src="https://i.imgur.com/ID133Rk.jpeg">

**2. Starup File và LD File.**  

**a. Startup File**    

![](https://i.imgur.com/obSjBTH.png)

- File này dùng để khởi tạo giá trị cho MSP, PC và chạy Reset_Handler (Cấu hình hệ thống, khởi tạo vùng nhớ .data, .bss, Copy dữ liệu,...)

**b. LD File - Linker Script**  

![](https://i.imgur.com/8qaO6po.png)

- File này dùng để xác định hàm khởi đầu của chương trình là Reset_Handler. Đặt kích thước và phân vùng, tức là xác định các phân vùng (.isr_vector, .text,...) nằm trong bộ nhớ Flash, còn (.data, .bss, .heap_stack) nằm trong bộ nhớ RAM.

__________________________________________________________________________________________________________________________________________________________________________

# Lesson 2: Registers Address & BIT-MASK. 

## A. BUS INTERFACE & REGISTERS ADDRESS.

### I. Bus interface.       
- Bus như là các dây thần kinh, truyền dữ liệu xuống và yêu cầu ngoại vi nào hoạt động. Nó đóng vai trò trong việc kết nối và trao đổi dữ liệu giữa các thành phần trong hệ thống.
   
![](https://i.imgur.com/q003rZp.png)

**1. I-Bus:** Phụ trách truyền dữ liệu từ bộ nhớ Flash -> CPU.    
**2. D-Bus:** Đảm nhận truyền dữ liệu giữa CPU, bộ nhớ (Flash, SRAM) và các thiết bị ngoại vi.       
**3. S-Bus:** Quản lý giao tiếp giữa các thành phần hệ thống như CPU, bộ nhớ và thiết bị ngoại vi.     
**4. P-Bus:** Kết nối và quản lý giao tiếp giữa CPU và các thiết bị ngoại vi.        
**5. AHB (Advanced High-performance Bus)**: Đây là bus chính, tốc độ cao nhất trong hệ thống.          
**6. APB1 (Advanced Peripheral Bus 1)**: Là bus ngoại vi tốc độ thấp hơn, thường được sử dụng cho các thiết bị ngoại vi không yêu cầu băng thông cao như UART, I2C, SPI.     
**7. APB2 (Advanced Peripheral Bus 2)**: Tương tự APB1 nhưng thường hoạt động ở tốc độ cao hơn. Nó được sử dụng cho các thiết bị ngoại vi yêu cầu tốc độ và băng thông cao hơn như ADC, một số Timer nhanh.     
**APB1 & APB2**: Thường được kết nối với AHB hoặc AHB2 thông qua một bridge

### II. Registers Address.      
**1. Công thức tính**
```cpp
    Register Address = Peripheral Base Address + Offset Address
```
**Trong đó:**  
- **Peripheral Base Address**: Đây là địa chỉ cơ sở của một ngoại vi trong bộ vi xử lý.
- **Register Offset**: Đây là một giá trị offset (độ dời) tính bằng byte, để xác định vị trí của một thanh ghi cụ thể trong vùng nhớ của ngoại vi.      
- **Register Address**: Đây là địa chỉ của một thanh ghi cụ thể trong ngoại vi.

***Ví dụ:* Truy cập vào thanh ghi RCC_CR (Clock Control Register)**

*Các thanh ghi RCC*
![](https://i.imgur.com/NDw4U5i.png)

**Base Address của RCC_CR**
![](https://i.imgur.com/VcoFZRC.png)

**Offset Address của RCC_CR**
![](https://i.imgur.com/FBCWYWn.png)

**Địa chỉ của RCC_CR**
```cpp
Register Address of RCC_CR = 0x40021000 + 0x00 = 0x40021000
```
**2. Thuộc tính các Bit**

![](https://i.imgur.com/ehRGiLa.png)

## A. Bit-Mask.
* Kĩ thuật này giúp ta Set, Clean hay Toggle 1 bit mà không ảnh hưởng đến các bit còn lại.
### 1. Set Bit - Thiết lập bit thành 1.
```cpp
              bitmask |= (1 << i)

              i: Bit vị trí i của bitmask cần bật, i tính từ 0.
```

**Ví dụ:** Bitmask = 0b0000, Set bit thứ 2.
```cpp
    Bitmask |= (1 << 2) => 0b0010

Chứng minh:
    Bitmask:            0   0   0   0
                OR(|)
                        0   0   1   0   =>  (1 << 1): 1 dạng nhị phân đã dịch sang phải 1 bit

    Result:             0   0   1   0

```
### 2. Clean Bit - Thiết lập bit thành 0.
```cpp
              bitmask &= ~(1 << i)

              i: Bit vị trí i của bitmask cần tắt, i tính từ 0.
```

**Ví dụ:** Bitmask = 0b1111, Clean bit thứ 2.
```cpp
    Bitmask &= ~(1 << 2) => 0b1101

Chứng minh:
    Bitmask:            1   1   1   1
                AND(&)
                        1   1   0   1   =>  ~(1 << 1): 1 dạng nhị phân đã dịch sang phải 1 bit và đảo ngược bit.
                        
    Result:             1   1   0   1

```
### 3. Toggle Bit - Chuyển đổi trạng thái của bit từ 0 -> 1 hoặc 1 -> 0.
```cpp
              bitmask ^= (1 << i)

              i: Bit thứ i của bitmask cần đổi trạng thái.
```
***Lưu ý:*** Thường thì khi làm việc với Bit ta cần biết rõ trạng thái của Bit. Việc sử dụng Toggle Bit khi không biết rõ trạng thái Bit là việc không nên.
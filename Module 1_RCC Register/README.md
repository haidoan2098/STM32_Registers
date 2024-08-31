### THỰC TẾ
Trong STM32, nó sẽ có sẵn thư viện đã khai báo đầy đủ hết, nên ta không cần khai báo lại. Nên ta cứ lấy thư viện đó mà dùng.    

***Hướng dẫn lấy file thư viện***   
**Bước 1**: Tạo Project dùng *HAL*.    

![](https://i.imgur.com/yE7eecD.png)      

**Bước 2**: Copy thư mục này rồi bỏ vào Project đang làm, chú ý đúng vị trí.        
**Bước 3**: Dán *Inlucde Paths* (Ở dòng Compiler).        
Chọn *Properties* tại Project cần làm, chọn *C/C++ Build* -> *Setting* dán path đến như hình.        
![](https://i.imgur.com/7t2rP8B.png)        

**Bước 4**: Apply -> Apply Close.       


### LUYỆN TẬP
Dùng: STM32F103C8T6.        
Cấu hình: Bật thạch anh ngoài (HSE) -> bộ chia 1 -> nguồn đầu vào là HSE -> chọn nhân tần thích hợp để SYSCLK = 72MHz -> Bus trên AHB 36MHZ -> APB1 Peripheral Clocks = 9MHz và APB1 Peripheral Clocks = 18MHz.


### BÀI TẬP 
Dùng: STM32F407VGT6.                      
Xây dựng thư viện Clock Config để MCU có tần số đạt giá trị tối đa (72MHz).
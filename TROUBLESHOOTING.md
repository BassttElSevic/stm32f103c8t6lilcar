# 故障排除指南

本文档提供常见问题的详细排查步骤和解决方案。

## 目录
- [编译问题](#编译问题)
- [烧录问题](#烧录问题)
- [硬件连接问题](#硬件连接问题)
- [电机问题](#电机问题)
- [PS2手柄问题](#ps2手柄问题)
- [电源问题](#电源问题)
- [性能问题](#性能问题)

---

## 编译问题

### 问题1：找不到头文件
```
error: stm32f1xx_hal_xxx.h: No such file or directory
```

**原因：** 缺少完整的STM32 HAL库

**解决方法：**
1. 使用STM32CubeMX生成完整的HAL库项目
2. 或从ST官网下载STM32F1 HAL库包
3. 将HAL库文件复制到 `Drivers/STM32F1xx_HAL_Driver/` 目录

**下载链接：**
- [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html)
- [STM32F1 HAL库](https://www.st.com/en/embedded-software/stm32cubef1.html)

### 问题2：编译器未找到
```
arm-none-eabi-gcc: command not found
```

**解决方法：**

**Linux/Ubuntu:**
```bash
sudo apt-get update
sudo apt-get install gcc-arm-none-eabi
```

**macOS:**
```bash
brew install gcc-arm-embedded
```

**Windows:**
1. 下载 [ARM GCC工具链](https://developer.arm.com/downloads/-/gnu-rm)
2. 安装并添加到系统PATH

### 问题3：链接错误
```
undefined reference to `xxx`
```

**可能原因：**
- HAL库函数未实现
- 缺少启动文件
- 缺少系统文件

**解决方法：**
1. 检查Makefile中的源文件列表
2. 确保所有必需的HAL源文件已包含
3. 添加缺失的 `.c` 文件到 `C_SOURCES`

---

## 烧录问题

### 问题1：ST-Link连接失败
```
Error: unable to open stlink device
```

**检查步骤：**
1. ✅ ST-Link是否正确连接到电脑
2. ✅ ST-Link驱动是否已安装
3. ✅ USB线缆是否正常
4. ✅ ST-Link上的指示灯是否亮起

**解决方法：**

**Windows:**
- 安装 [ST-Link驱动](https://www.st.com/en/development-tools/stsw-link009.html)
- 使用设备管理器检查驱动状态

**Linux:**
```bash
# 安装stlink工具
sudo apt-get install stlink-tools

# 添加udev规则
sudo cp /usr/share/doc/stlink-tools/examples/49-stlinkv*.rules /etc/udev/rules.d/
sudo udevadm control --reload-rules
```

### 问题2：芯片连接失败
```
Error: init mode failed
```

**检查步骤：**
1. ✅ SWDIO连接到PA13
2. ✅ SWCLK连接到PA14
3. ✅ GND连接正确
4. ✅ STM32供电正常（3.3V）

**解决方法：**
1. 按住BOOT0按键
2. 按下并释放RESET按键
3. 释放BOOT0
4. 重新尝试烧录

### 问题3：烧录后程序不运行

**检查：**
- BOOT0引脚是否接地（应该接GND）
- 复位后是否正常启动

**解决：**
```bash
# 重新烧录并复位
st-flash write build/stm32f103c8t6lilcar.bin 0x8000000
st-flash reset
```

---

## 硬件连接问题

### 问题：如何验证接线是否正确？

**分步验证：**

#### 第1步：电源测试
```bash
# 使用万用表测量
1. STM32的3.3V引脚 → 应该是3.3V
2. STM32的5V引脚 → 应该是5V
3. L298N的5V输出 → 应该是5V
```

#### 第2步：GPIO测试
创建简单的LED闪烁程序测试GPIO：
```c
// 测试代码
while(1) {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
    HAL_Delay(500);
}
```

#### 第3步：SPI通信测试
- 用示波器或逻辑分析仪查看SPI信号
- 检查CLK、MISO、MOSI波形

### 检查清单

#### L298N连接检查
- [ ] IN1-IN4 → 对应的GPIO引脚
- [ ] ENA、ENB → PWM输出引脚
- [ ] 电机连接到OUT1-OUT4
- [ ] GND共地
- [ ] 电机电源连接正确
- [ ] 跳线帽安装在ENA/ENB上

#### PS2接收器检查
- [ ] VCC → 3.3V（不是5V！）
- [ ] GND → GND
- [ ] SCK → PA5
- [ ] MISO → PA6
- [ ] MOSI → PA7
- [ ] CS → PA4

---

## 电机问题

### 问题1：电机不转

**可能原因和解决方法：**

#### 原因A：L298N跳线帽未安装
**检查：** L298N的ENA和ENB位置是否有跳线帽
**解决：** 安装跳线帽或直接用线连接ENA/ENB到对应的PWM引脚

#### 原因B：电源不足
**检查：**
```bash
# 用万用表测量
1. 电池电压 → 应该大于7V
2. L298N的12V输入 → 应该等于电池电压
3. 负载时的电压降 → 不应该太大
```
**解决：** 使用容量更大的电池或充电

#### 原因C：PWM信号问题
**调试代码：**
```c
// 在main.c中添加测试代码
L298N_SetDirection(MOTOR_FL, DIR_FORWARD);
L298N_SetSpeed(MOTOR_FL, 1000);  // 100%速度
while(1);  // 持续运行
```

#### 原因D：电机驱动损坏
**测试方法：**
1. 断开STM32连接
2. 手动将IN1接高电平，IN2接地
3. 手动给ENA接高电平
4. 电机应该转动

### 问题2：电机转向错误

**情况A：所有电机转向相反**
- 前进变后退，后退变前进

**解决：** 修改代码中的方向定义
```c
// 在l298n.c中交换
case DIR_FORWARD:
    HAL_GPIO_WritePin(..., GPIO_PIN_RESET);  // 改为RESET
    HAL_GPIO_WritePin(..., GPIO_PIN_SET);    // 改为SET
```

**情况B：单个电机转向错误**
- 某个电机转向与其他不同

**解决：** 交换该电机的两根线

### 问题3：电机抖动或无力

**可能原因：**
1. PWM频率不合适
2. 电源电压不够
3. 负载过大
4. 电机故障

**解决方法：**

**调整PWM频率：**
```c
// 在main.c中修改
htim2.Init.Prescaler = 71;   // 增大预分频器降低频率
htim2.Init.Period = 999;     // 调整周期
```

**检查电压：**
- 测量运行时的电压
- 应该保持在7V以上

### 问题4：电机过热

**原因：**
- 负载过大
- L298N驱动过热
- 电机堵转

**解决：**
1. 为L298N添加散热片
2. 降低工作占空比
3. 检查机械结构是否卡顿

---

## PS2手柄问题

### 问题1：手柄无法配对

**配对步骤：**
1. 给接收器通电（3.3V）
2. 打开手柄（安装电池）
3. 同时按住START和SELECT键3秒
4. 观察指示灯：闪烁→常亮表示成功

**注意事项：**
- 不同型号手柄配对方法可能不同
- 有些需要按MODE键
- 查看手柄说明书

### 问题2：手柄已配对但无响应

**检查步骤：**

#### 1. 电源检查
```
⚠️ 重要：PS2接收器必须使用3.3V，不能用5V！
```
- 用万用表测量接收器的VCC引脚
- 应该是3.3V ±0.1V

#### 2. SPI通信检查

**添加调试代码：**
```c
// 在main.c的主循环中
PS2_ReadData(&ps2_data);

// 检查返回值
if (ps2_data.buttons == 0 && 
    ps2_data.left_x == 0x80 && 
    ps2_data.left_y == 0x80) {
    // 正常接收
} else {
    // 可能有问题
}
```

#### 3. 接线检查
```
STM32  →  PS2接收器
PA4    →  ATT (CS)
PA5    →  CLK
PA6    →  DAT (MISO)
PA7    →  CMD (MOSI)
```

### 问题3：手柄有时响应有时不响应

**可能原因：**
1. 无线信号干扰
2. 手柄电池电量低
3. 距离太远

**解决方法：**
1. 更换手柄电池
2. 减小手柄与接收器的距离
3. 远离WiFi路由器等干扰源
4. 增加轮询延时：
```c
HAL_Delay(20);  // 改为30或50
```

### 问题4：摇杆数据不准确

**现象：**
- 摇杆回中但小车仍在移动
- 摇杆数据漂移

**解决：** 调整死区阈值
```c
// 在config.h中修改
#define PS2_DEADZONE_THRESHOLD  30  // 增大死区
```

---

## 电源问题

### 问题1：上电后立即重启

**可能原因：**
- 电流过大导致电压跌落
- 电源容量不足
- 短路

**解决方法：**
1. 检查是否有短路
2. 使用容量更大的电池
3. 分开电机电源和STM32电源

### 问题2：运行一段时间后停止

**可能原因：**
- 电池耗尽
- 过热保护
- 连接松动

**解决：**
1. 充电或更换电池
2. 添加散热片
3. 检查所有连接

### 问题3：STM32无法供电

**检查：**
1. 5V输入是否正常
2. 3.3V稳压器是否工作
3. 板上保险丝是否烧断

---

## 性能问题

### 问题1：响应延迟大

**原因分析：**
- PS2轮询频率太低
- 主循环延时太长

**优化方法：**
```c
// 减小延时
HAL_Delay(10);  // 从20ms改为10ms

// 调整死区
#define PS2_DEADZONE_THRESHOLD  15  // 减小死区
```

### 问题2：转向不灵活

**调整方法：**
```c
// 在l298n.c的TurnLeft函数中
// 增加内外侧车轮的速度差

// 左转时
L298N_SetSpeed(MOTOR_FL, speed * 0.5);  // 左侧减速
L298N_SetSpeed(MOTOR_RL, speed * 0.5);
L298N_SetSpeed(MOTOR_FR, speed);        // 右侧全速
L298N_SetSpeed(MOTOR_RR, speed);
```

### 问题3：速度不够快

**检查：**
1. 电池电压是否充足
2. PWM占空比是否达到100%
3. 负载是否过重

**优化：**
```c
// 增加默认速度
#define MOTOR_DEFAULT_SPEED  900  // 提高到90%
```

---

## 高级调试技巧

### 1. 使用串口调试

**添加UART支持：**
```c
// 初始化UART
// PA9: TX, PA10: RX

// 输出调试信息
printf("Motor Speed: %d\n", speed);
printf("PS2 Buttons: 0x%04X\n", ps2_data.buttons);
```

### 2. 使用示波器/逻辑分析仪

**检查信号：**
- PWM波形：应该是方波，频率1kHz
- SPI通信：CLK、MISO、MOSI、CS时序
- GPIO电平：高低电平切换

### 3. 单步调试

使用ST-Link和GDB：
```bash
# 启动OpenOCD
openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg

# 在另一个终端启动GDB
arm-none-eabi-gdb build/stm32f103c8t6lilcar.elf
(gdb) target remote localhost:3333
(gdb) load
(gdb) break main
(gdb) continue
```

---

## 获取帮助

如果以上方法都无法解决问题：

1. **检查GitHub Issues**
   - 搜索是否有类似问题
   - 查看已关闭的Issues

2. **提交新Issue**
   - 详细描述问题
   - 包含错误信息
   - 说明已尝试的解决方法
   - 提供硬件照片（如果是硬件问题）

3. **社区支持**
   - STM32论坛
   - Arduino社区
   - 电子DIY论坛

---

## 预防性维护

### 日常检查
- [ ] 检查所有连接是否松动
- [ ] 检查电池电量
- [ ] 检查L298N温度
- [ ] 检查电机运转声音

### 定期维护
- [ ] 清理灰尘
- [ ] 检查导线绝缘
- [ ] 润滑电机（如需要）
- [ ] 更新固件（如有新版本）

### 安全建议
- [ ] 首次测试时架空小车
- [ ] 准备紧急停止开关
- [ ] 避免在易燃物附近测试
- [ ] 儿童使用时需成人监护

---

## 联系支持

- GitHub: [提交Issue](https://github.com/BassttElSevic/stm32f103c8t6lilcar/issues)
- Email: 见README.md

记住：大多数问题都是接线错误或配置问题，仔细检查通常能解决！

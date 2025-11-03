# 快速开始指南

本指南帮助您快速搭建和运行STM32F103C8T6小车控制系统。

## 第一步：准备硬件

### 必需硬件清单
- [ ] 1x STM32F103C8T6开发板（Blue Pill）
- [ ] 2x L298N电机驱动模块
- [ ] 4x TT直流电机
- [ ] 1x PS2无线手柄套装（手柄+接收器）
- [ ] 1x 7.4V或11.1V锂电池
- [ ] 1x 四轮小车底盘
- [ ] 若干杜邦线
- [ ] 1x ST-Link V2烧录器

### 工具清单
- [ ] 电烙铁（可选，用于焊接）
- [ ] 万用表
- [ ] 螺丝刀

## 第二步：硬件连接

### 快速接线表

#### L298N #1 → STM32
```
IN1  → PA0      ENA  → PA15
IN2  → PA1      ENB  → PB3
IN3  → PA2      GND  → GND
IN4  → PA3      +12V → 电池+
```

#### L298N #2 → STM32
```
IN1  → PB0      ENA  → PB8
IN2  → PB1      ENB  → PB9
IN3  → PB6      GND  → GND
IN4  → PB7      +12V → 电池+
```

#### PS2接收器 → STM32
```
VCC → 3.3V      CLK → PA5
GND → GND       DAT → PA6
ATT → PA4       CMD → PA7
```

#### 电机连接
```
L298N #1:
  OUT1/OUT2 → 左前电机
  OUT3/OUT4 → 右前电机

L298N #2:
  OUT1/OUT2 → 左后电机
  OUT3/OUT4 → 右后电机
```

**详细接线图请参考 [HARDWARE.md](HARDWARE.md)**

## 第三步：配对PS2手柄

1. 打开PS2手柄电源（安装电池）
2. 给PS2接收器通电
3. 按住手柄上的"START"和"SELECT"键约3秒
4. 观察手柄指示灯，闪烁后变为常亮表示配对成功
5. 不同型号手柄配对方法可能略有不同，请查看手柄说明书

## 第四步：编译和烧录程序

### 安装工具链

#### Linux/Mac
```bash
# 安装ARM GCC工具链
sudo apt-get install gcc-arm-none-eabi  # Ubuntu/Debian
# 或
brew install gcc-arm-embedded            # macOS

# 安装烧录工具
sudo apt-get install stlink-tools       # Ubuntu/Debian
```

#### Windows
1. 下载并安装 [ARM GCC工具链](https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm)
2. 下载并安装 [ST-Link Utility](https://www.st.com/en/development-tools/stsw-link004.html)

### 编译固件
```bash
# 进入项目目录
cd stm32f103c8t6lilcar

# 编译项目
make

# 编译成功后会在build目录下生成.bin和.hex文件
```

### 烧录固件

#### 使用ST-Link
```bash
# 连接ST-Link到STM32
# SWDIO → PA13
# SWCLK → PA14
# GND   → GND
# 3.3V  → 3.3V

# 烧录
make flash
# 或
st-flash write build/stm32f103c8t6lilcar.bin 0x8000000
```

#### 使用OpenOCD
```bash
openocd -f interface/stlink-v2.cfg -f target/stm32f1x.cfg \
  -c "program build/stm32f103c8t6lilcar.elf verify reset exit"
```

## 第五步：测试系统

### 安全检查
1. ✅ 确认所有接线正确
2. ✅ 确认电机未固定，可以自由转动
3. ✅ 确认PS2手柄已配对
4. ✅ 确认电池电压在安全范围内（7.4V-12V）

### 功能测试

#### 1. 电源测试
- 给STM32上电
- 观察板载LED是否点亮
- 用万用表检查3.3V和5V输出

#### 2. PS2通信测试
- PS2接收器上电
- 打开PS2手柄
- 手柄指示灯应该常亮（已配对状态）

#### 3. 电机测试
⚠️ **重要：首次测试时架空小车，避免意外移动**

1. 给L298N供电（7.4V-12V）
2. 按PS2手柄的"↑"键
3. 四个电机应该同时正转
4. 松开按键，电机停止

#### 4. 完整功能测试
| 操作 | 预期结果 |
|-----|---------|
| 按↑键 | 小车前进 |
| 按↓键 | 小车后退 |
| 按←键 | 小车左转 |
| 按→键 | 小车右转 |
| 推左摇杆上 | 小车前进（速度可调）|
| 推左摇杆下 | 小车后退（速度可调）|
| 推左摇杆左 | 小车左转（速度可调）|
| 推左摇杆右 | 小车右转（速度可调）|
| 摇杆回中 | 小车停止 |

## 常见问题排查

### 问题1：电机不转
**可能原因：**
- L298N跳线帽未安装在ENA/ENB上
- 电机供电不足
- 接线错误

**解决方法：**
1. 检查L298N的ENA和ENB跳线帽是否安装
2. 用万用表测量电机供电电压
3. 重新检查接线

### 问题2：手柄无响应
**可能原因：**
- 手柄未配对
- PS2接收器供电不正确
- SPI通信故障

**解决方法：**
1. 重新配对手柄
2. 确认PS2接收器使用3.3V供电（不是5V！）
3. 检查SPI接线（PA4、PA5、PA6、PA7）

### 问题3：方向错误
**可能原因：**
- 电机极性接反
- 电机位置安装错误

**解决方法：**
1. 交换对应电机的两根线
2. 检查电机安装位置（参考HARDWARE.md中的布局图）

### 问题4：编译错误
**可能原因：**
- 缺少HAL库文件
- 工具链未正确安装

**解决方法：**
1. 本项目使用精简的HAL头文件，完整功能需要STM32 HAL库
2. 建议使用STM32CubeMX生成完整的HAL库代码
3. 或从ST官网下载完整的STM32F1 HAL库

### 问题5：烧录失败
**可能原因：**
- ST-Link连接不良
- STM32处于锁定状态
- 驱动未正确安装

**解决方法：**
1. 检查ST-Link连接
2. 尝试按住BOOT0按键，然后复位
3. Windows下需要安装ST-Link驱动

## 进阶配置

### 调整速度参数
编辑 `Core/Src/main.c` 文件：
```c
// 第99行附近
uint16_t speed = 700;  // 修改默认速度 (0-1000)
```

### 调整死区阈值
编辑 `Core/Src/main.c` 文件：
```c
// 第117行附近
if (left_y > 20 || left_y < -20 || left_x > 20 || left_x < -20) {
    // 修改20为其他值调整死区大小
}
```

### 调整PWM频率
编辑 `Core/Src/main.c` 文件：
```c
// 第219和261行附近
htim2.Init.Prescaler = 71;  // 预分频器
htim2.Init.Period = 999;    // 周期 (修改这两个值调整PWM频率)
```

## 下一步

完成基本功能后，您可以：
1. 添加超声波传感器实现避障
2. 添加蓝牙模块实现手机控制
3. 添加舵机云台实现摄像头控制
4. 添加循迹传感器实现自动循迹
5. 实现PID控制提高运动精度

## 获取帮助

如果遇到问题：
1. 查看 [README.md](README.md) 了解项目概述
2. 查看 [HARDWARE.md](HARDWARE.md) 了解详细接线
3. 检查接线和配置是否正确
4. 在GitHub仓库提交Issue

## 安全提示

⚠️ **请注意：**
- 首次测试时务必架空小车
- 确保周围没有障碍物
- 电机驱动模块会发热，长时间运行需要散热
- 避免短路
- 正确处理锂电池，避免过充过放

祝您玩得开心！🚗💨

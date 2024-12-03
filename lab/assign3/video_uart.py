import os
import cv2
import numpy as np
import serial
import time  # 导入time模块用于等待
import struct

from tqdm import tqdm

# 视频文件的路径
video_path = r"..\..\lectures\video_material.mp4"
# 创建视频读取对象
cap = cv2.VideoCapture(video_path)

# 检查视频是否成功打开
if not cap.isOpened():
  print("Error: Could not open video.")
  exit()

# 获取视频的帧率
fps = cap.get(cv2.CAP_PROP_FPS)
print("Video Loaded. FPS: %.2f" % fps)

# 计算每秒需要处理的帧数
frames_per_second = 8

# 计算每帧的时间间隔（以帧为单位）
frame_interval = int(fps / frames_per_second)

# 创建串口对象
ser = serial.Serial("COM11", 256000, 8, "N", 1, timeout=None)
ser.dtr = False
ser.rts = False
print("Serial port opened.")

# 存储处理后的帧数据
all_frame_data = []

# 读取视频帧
frame_count = 0
frame_count_target = 256
with tqdm(total=frame_count_target, desc="Processed Frames", unit="frame") as pbar:
  while True:
    ret, frame = cap.read()
    if (not ret) or (frame_count >= frame_count_target):
      break

    # 检查是否是我们需要处理的帧
    if frame_count % frame_interval == 0:
      # 转换为RGB颜色空间
      frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

      # 缩放图片到45x60
      resized_frame = cv2.resize(frame_rgb, (60, 45), interpolation=cv2.INTER_AREA)

      # 获取图像的尺寸
      height, width = resized_frame.shape[:2]

      # 创建一个空的数组来存储16位的像素值
      frame_16bit = np.zeros((height, width), dtype=np.uint16)

      # 将RGB图像转换为16位真色彩（RGB565格式）
      for y in range(height):
        for x in range(width):
          r = np.uint16((resized_frame[y, x, 0] >> 3)) << 11  # 红色5位
          g = np.uint16((resized_frame[y, x, 1] >> 2)) << 5  # 绿色6位
          b = (resized_frame[y, x, 2] >> 3) & 0x1F  # 蓝色5位
          frame_16bit[y, x] = r | g | b

      # 将图像转换为一维数组
      flat_array = frame_16bit.flatten()

      # 存储处理后的帧数据
      all_frame_data.append(flat_array)

    frame_count += 1
    pbar.update(1)

ser_frame_length = 150

for fr in all_frame_data:
  serial_data_array = np.array(fr, dtype=np.uint16)
  serial_data_array = serial_data_array.reshape((serial_data_array.__len__() // ser_frame_length, ser_frame_length))
  serial_data_array = [x.tolist() for x in serial_data_array]
  # 发送处理后的帧数据
  for frame_index, frame_data in enumerate(serial_data_array):
    # 发送固定标志位
    frame_marker = 0x1234
    frame_data.insert(0, len(frame_data))
    frame_data.insert(0, frame_marker)
    frame_data.append(0x0D0A)
    # 发送帧数据
    for value in frame_data:
      ser.write(value.to_bytes(2, byteorder="little"))  # 发送16位数据
    # 换行符表示帧结束
    # ser.write(0x0D0A.to_bytes(2, byteorder="little"))
    print("".join(f"{value:x}" for value in frame_data))
    # time.sleep(2)
    ser.timeout = 0.001
    raw_data = ser.read(2)
    if raw_data:
      index = struct.unpack("<2B", raw_data)
      print(index)
    break
    print("Frame %d sent." % frame_index)
  break

# 释放视频读取对象
cap.release()
# 关闭串口
ser.close()

print("所有帧已发送完毕。")

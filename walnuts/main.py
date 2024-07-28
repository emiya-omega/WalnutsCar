import board
#from cushy_serial import CushySerial
import serial
import time
import cv2
import numpy as np

cap = cv2.VideoCapture(1)
cap.set(3,640) #设置分辨率
cap.set(4,480)
com = serial.Serial("/dev/ttyS2",9600)


while (1):
    ret, frame = cap.read()
    # 转化为灰度图
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    # 大津法二值化
    retval, dst = cv2.threshold(gray, 0, 255, cv2.THRESH_OTSU)
    # 膨胀，白区域变大
    dst = cv2.dilate(dst, None, iterations=2)
    # # 腐蚀，白区域变小
    # dst = cv2.erode(dst, None, iterations=6)

    # 单看第400行的像素值s
    color = dst[400]
    # 找到白色的像素点个数
    white_count = np.sum(color == 255)
    # 找到白色的像素点索引
    white_index = np.where(color == 255)

    # 防止white_count=0的报错
    if white_count == 0:
        white_count = 1

    # 找到黑色像素的中心点位置
    if white_count != 1:
        center_now = (white_index[0][white_count - 1] + white_index[0][0]) / 2
    else:
        center_now = 320
    # 计算出center_now与标准中心点的偏移量
    direction =  center_now - 320

    print(center_now)
    
          
          
       
    com.write("@".encode()) 
    com.write((str(float(center_now))).encode())
  #  com.write((str(float(direction))).encode())  # 将整数转换为字节串并写入
 # 将整数转换为字节串并写入
    com.write("\r\n".encode())

    com.flushInput()
    time.sleep(0.1)

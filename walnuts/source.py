import time
import cv2
import numpy as np
 
def sign(x):
    if x>0:
        return 1.0
    else:
        return -1.0
 
 
# center定义
center_now = 320
# 打开摄像头，图像尺寸640*480（长*高），opencv存储值为480*640（行*列）
cap = cv2.VideoCapture(1)
 
# PID 初始数据
error = [0.0] * 3
adjust = [0.0] * 3
# PID 参数配置
kp = 1.5
ki = 0.4
kd = 0.1
target = 320
 
while (1):
    ret, frame = cap.read()
    # 转化为灰度图
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    # 大津法二值化
    retval, dst = cv2.threshold(gray, 0, 255, cv2.THRESH_OTSU)
    # 膨胀，白区域变大
    #dst = cv2.dilate(dst, None, iterations=2)
     # 腐蚀，白区域变小
    dst = cv2.erode(dst, None, iterations=6)
    cv2.imshow("video",dst)
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
    center_now = (white_index[0][white_count - 1] + white_index[0][0]) / 2
 
    # 计算出center_now与标准中心点的偏移量
    direction = center_now - 320
 
    #print(direction)
 
    # 停止
 #   if abs(direction) > 250:
 #       pwm1.ChangeDutyCycle(0)
       # pwm2.ChangeDutyCycle(0)
  #      pwm3.ChangeDutyCycle(0)
      #  pwm4.ChangeDutyCycle(0)
 
    # 更新PID误差
    error[1] = error[2]
    error[2] = center_now - target
 
    # 更新PID输出（增量式PID表达式）
    adjust[0] = adjust[1]
    adjust[1] = adjust[2]
    #adjust(k+2) = adjust(k+1) + kp * (e(k+2) - e(k+1)) + ki * e(k+2) + kd * (e(k+2)-2*e(k+1)+e(k))

    adjust[2] = adjust[1] \
        + kp*(error[2] - error[1]) \
        + ki*error[2] \
        + kd*(error[2] - 2*error[1] + error[0]); 
 
    # 饱和输出限制在70绝对值之内
    if abs(adjust[2]) > 70:
        adjust[2] = sign(adjust[2]) * 70
 
    print(adjust[2])
    # 执行PID
 
    # 右转
  #  elif adjust[2] > 0:
   #     pwm1.ChangeDutyCycle(30+ adjust[2])
   #    # pwm2.ChangeDutyCycle(0)
   #     pwm3.ChangeDutyCycle(30)
      #  pwm4.ChangeDutyCycle(0)
 
    # 左转
  #  elif adjust[2] < 0:
  #      pwm1.ChangeDutyCycle(30)
       # pwm2.ChangeDutyCycle(0)
   #     pwm3.ChangeDutyCycle(30 + abs(adjust[2]))
       # pwm4.ChangeDutyCycle(0)
 
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
    else:
        time.sleep(0.2)
 
# 释放清理
cap.release()
cv2.destroyAllWindows()
#pwm1.stop()
#pwm2.stop()
#pwm3.stop()
#pwm4.stop()
#gpio.cleanup()
import cv2
import numpy as np

def empty(v):
    pass

def adjust_brightness(image, alpha, beta):
    # 调整图像亮度的函数
    new_image = cv2.convertScaleAbs(image, alpha=alpha, beta=beta)
    return new_image
def adjust_saturation(image, saturation):
    # 调整图像饱和度的函数
    hsv_image = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
    hsv_image[:, :, 1] = hsv_image[:, :, 1] * saturation
    new_image = cv2.cvtColor(hsv_image, cv2.COLOR_HSV2BGR)
    return new_image
def adjust_contrast(image, alpha):
    # 调整图像对比度的函数
    new_image = cv2.convertScaleAbs(image, alpha=alpha)
    return new_image

#加載視頻
video = cv2.VideoCapture('1b2r_armors.mp4')

print("Please select the mode: \n'0' means to identify by adjusting the HSV parameters yourself, \n'1' means to use the fixed red HSV parameters to identify, \n'2' means to use the fixed blue HSV parameters to identify.")

#自己調參用
#Hue控制條
cv2.namedWindow('TrackBar')      #新建window視窗
cv2.resizeWindow('TrackBar', 640, 320)      #set大小, 寛, 高
cv2.createTrackbar('Mode', 'TrackBar', 0, 1, empty)     #模式选择
cv2.createTrackbar('control', 'TrackBar', 0, 1, empty)
while True:  #不停取得HSV各值 (Hue, Saturation, Value)
    mode = cv2.getTrackbarPos('Mode', 'TrackBar')   #控制條名, 視窗名
    control = cv2.getTrackbarPos('control', 'TrackBar')
    cv2.waitKey(1)
    if control == 1:
        break

# 调整亮度、饱和度和对比度的参数
brightness_alpha = 0.5  # 亮度增益,大于1增加亮度，小于1降低亮度
brightness_beta = 0  # 偏移值，增加或减少亮度的常数
saturation_factor = 1.5  # 饱和度因子，大于1增加饱和度，小于1降低饱和度
contrast_alpha = 1.5  # 对比度增益，大于1增加对比度，小于1降低对比度

if mode == 0:
    lower = np.array([173, 74, 62])   #紅色HSV
    upper = np.array([179, 233, 222])
    saturation_factor = 2  #識別紅色灯條时把飽和度調高能提升準确率

elif mode == 1:
    lower = np.array([90, 90, 89])  #藍色HSV
    upper = np.array([119, 193, 181])
    '''lower = np.array([96, 0, 97])  #藍色HSV
    upper = np.array([108, 137, 255])'''

while True:
    ret, frame = video.read()
    if ret:
        #img = cv2.resize(img, (0,0), fx = 1.2, fy = 1.2)
        cv2.imshow('video.mp4', frame)
        # 调整亮度、饱和度和对比度
        adjusted_frame = adjust_brightness(frame, brightness_alpha, brightness_beta)
        adjusted_frame = adjust_saturation(adjusted_frame, saturation_factor)
        adjusted_frame = adjust_contrast(adjusted_frame, contrast_alpha)

        cv2.imshow('Adjusted Video', adjusted_frame)
        hsv = cv2.cvtColor(adjusted_frame, cv2.COLOR_BGR2HSV)
        #過濾顏色
        mask = cv2.inRange(hsv, lower, upper)   
        #套用到原圖
        result = cv2.bitwise_and(adjusted_frame, adjusted_frame, mask = mask)  
        imgContour = result.copy()  
        #偵測輪廓不需要顏色
        new_img = cv2.cvtColor(result, cv2.COLOR_BGR2GRAY)   
        #偵測邊緣
        canny = cv2.Canny(new_img, 50, 150)   

        kernal = np.ones((2,2), np.uint8) 
        #澎漲
        dilate = cv2.dilate(canny, kernal, iterations=1) 
        #澎漲效果展示
        cv2.imshow('dilate', dilate)  

        contours, hierarchy = cv2.findContours(dilate, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)    #偵測輪廓

        i = 1 
        for cnt in contours:
            #畫出輪廓
            cv2.drawContours(imgContour, cnt, -1, (255, 0, 0), 1)   
            #輪廓面積
            area = cv2.contourArea(cnt)    
            #輪廓周長
            peri = cv2.arcLength(cnt, False)    
            #近似輪廓(回傳近似多邊形的頂點)
            vertices = cv2.approxPolyDP(cnt, peri*0.018, False)     
            #頂點數量
            corners = len(vertices)     
            #虛拟畫一方形框住所偵測之圖形(返回四個值：左上坐标、寛度、高度)
            x, y, w, h = cv2.boundingRect(vertices)     
            #高闊比
            ratio = h/w   
            #對高闊比、面積作出限制, 過濾掉不符項
            if 0.85 <= ratio <= 2.5:  
                if 18 <= area <= 200:
                    cv2.rectangle(adjusted_frame, (x, y), (x+w, y+h), (0, 0, 255), 2) 
                    cv2.putText(adjusted_frame, str(i), (x, y-5), cv2.FONT_HERSHEY_COMPLEX, 0.5, (0, 0, 255), 1)
                    print('Ratio', i, '.', ratio)
                    print('Area', i, '.', area)
                    i += 1
        #成果展示
        cv2.imshow('result', adjusted_frame)  
        #過程展示
        cv2.imshow('canny', canny)  
        cv2.imshow('imgContour', imgContour) 
        if cv2.waitKey(20) == ord(' '):
            cv2.waitKey(0)
    else:
        video.release()
        #output.release()
        cv2.destroyAllWindows()
        break
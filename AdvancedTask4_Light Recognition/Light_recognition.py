import cv2
import numpy as np

def empty(v):
    pass

img = cv2.imread('armor.png')
img = cv2.resize(img, (0,0), fx = 0.7, fy = 0.7)

print("Please select the mode: \n'0' means to identify by adjusting the HSV parameters yourself, \n'1' means to use the fixed red HSV parameters to identify, \n'2' means to use the fixed blue HSV parameters to identify.")

#自己調參用
#Hue控制條
cv2.namedWindow('TrackBar')      #新建window視窗
cv2.resizeWindow('TrackBar', 640, 320)      #set大小, 寛, 高
cv2.createTrackbar('Mode', 'TrackBar', 0, 2, empty)     #模式选择
cv2.createTrackbar('Hue Min', 'TrackBar', 0, 179, empty)        
cv2.createTrackbar('Hue Max', 'TrackBar', 179, 179, empty)  
cv2.createTrackbar('Sat Min', 'TrackBar', 0, 255, empty)   
cv2.createTrackbar('Sat Max', 'TrackBar', 255, 255, empty)   
cv2.createTrackbar('Val Min', 'TrackBar', 0, 255, empty)     
cv2.createTrackbar('Val Max', 'TrackBar', 255, 255, empty)  
cv2.createTrackbar('control', 'TrackBar', 0, 1, empty)
hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)  
while True:  #不停取得HSV各值 (Hue, Saturation, Value)
    mode = cv2.getTrackbarPos('Mode', 'TrackBar') 
    h_min = cv2.getTrackbarPos('Hue Min', 'TrackBar')   
    h_max = cv2.getTrackbarPos('Hue Max', 'TrackBar')
    s_min = cv2.getTrackbarPos('Sat Min', 'TrackBar')
    s_max = cv2.getTrackbarPos('Sat Max', 'TrackBar')
    v_min = cv2.getTrackbarPos('Val Min', 'TrackBar')
    v_max = cv2.getTrackbarPos('Val Max', 'TrackBar')
    control = cv2.getTrackbarPos('control', 'TrackBar')
    print(h_min, h_max, s_min, s_max, v_min, v_max)
    lower = np.array([h_min, s_min, v_min])
    upper = np.array([h_max, s_max, v_max])
    #過濾顏色
    mask = cv2.inRange(hsv, lower, upper)   
    #套用到原圖
    result = cv2.bitwise_and(img, img, mask = mask) 
    cv2.imshow('armor.png', img)
    cv2.imshow('mask', mask)
    cv2.waitKey(1)
    if control == 1:
        break

if mode == 0:
    pass

elif mode == 1:
    lower = np.array([173, 81, 200])   #紅色HSV
    upper = np.array([179, 255, 255])

elif mode == 2:
    lower = np.array([92, 39, 236])  #藍色HSV
    upper = np.array([102, 255, 255])

hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
#過濾顏色
mask = cv2.inRange(hsv, lower, upper)  
#套用到原圖
result = cv2.bitwise_and(img, img, mask = mask)  
imgContour = result.copy()  
#偵測輪廓不需要顏色
new_img = cv2.cvtColor(result, cv2.COLOR_BGR2GRAY)   
canny = cv2.Canny(new_img, 50, 150)   #偵測邊緣

kernal = np.ones((2,2), np.uint8)  
#澎漲
dilate = cv2.dilate(canny, kernal, iterations=1)
#澎漲效果展示
cv2.imshow('dilate', dilate)  
#偵測輪廓
contours, hierarchy = cv2.findContours(dilate, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)    

i = 1 
for cnt in contours:
    #畫出輪廓
    cv2.drawContours(imgContour, cnt, -1, (255, 0, 0), 1)   
    #輪廓面積
    area = cv2.contourArea(cnt)    
    #輪廓周長
    peri = cv2.arcLength(cnt, False)    
    #近似輪廓(調參)
    vertices = cv2.approxPolyDP(cnt, peri*0.018, False)    
    corners = len(vertices) #頂點數量
    #虛拟畫一方形框住所偵測之圖形(返回四個值：左上坐标、寛度、高度)
    x, y, w, h = cv2.boundingRect(vertices)     
    #高闊比
    ratio = h/w   
    #對高闊比、面積作出限制, 過濾掉不符項
    if 1.3 <= ratio <= 2.5:  
        if area >= 50:
            cv2.rectangle(img, (x, y), (x+w, y+h), (0, 0, 255), 2) 
            cv2.putText(img, str(i), (x, y-5), cv2.FONT_HERSHEY_COMPLEX, 0.5, (0, 0, 255), 1)
            print('Ratio', i, '.', ratio)
            print('Area', i, '.', area)
            i += 1
#成果展示
cv2.imshow('result', img)  
#過程展示
cv2.imshow('canny', canny)  
cv2.imshow('imgContour', imgContour) 
cv2.waitKey(0)
cv2.destroyAllWindows
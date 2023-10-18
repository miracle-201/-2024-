import cv2

img = cv2.imread('QRcode.png')
img = cv2.resize(img, (0,0), fx = 0.8, fy = 0.8)
imgContour = img.copy()

hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)  
#偵測邊緣(最低和最高門檻)
canny = cv2.Canny(hsv, 200, 250)  
#偵測輪廓,會回傳兩個值(輪廓和阶层(暫時用不著))：偵測之圖片、模式-外或內輪廓、近似方法-用於壓縮水平或垂直輪廓點-此保留所有輪廓點故不做壓縮
contours, hierarchy = cv2.findContours(canny, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)    
#在原圖的複制本imgContours上繪制輪廓
cv2.drawContours(imgContour, contours, -1, (255, 0, 0), 2)

for cnt in contours:
    area = cv2.contourArea(cnt)    #輪廓面積
    peri = cv2.arcLength(cnt, True)    #輪廓周長：輪廓、是否閉合
    vertices = cv2.approxPolyDP(cnt, peri*0.054, True)     #近似輪廓(回傳近似多邊形的頂點)：輪廓、近似值(越大多邊形邊越多, 反之少)、是否閉合
    corners = len(vertices)     #頂點數量
    x, y, w, h = cv2.boundingRect(vertices)     
    if 4 == corners: #過濾其他形狀留下正方形
        if 600 <= area <= 650: #限定正方形大小, 目标正方形面積為625左右
            if x <= 50 and y <= 50: #限制正方形位置, 目标正方形坐标為(34,34)
                ratio = h/w #計算長闊比
                if 0.95 <= ratio <= 1.05: #限制長闊比
                    cv2.rectangle(img, (x, y), (x+w, y+h), (255, 0, 0), 2)  #畫一方形框住所偵測之圖形(返回四個值：左上坐标、寛度、高度)
                    print("Corners:%d \nArea:%.2f \n(x,y):(%d,%d)"%(corners, area, x, y))

cv2.imshow('img', img)
cv2.imshow('canny', canny)
cv2.imshow('imgContour', imgContour)
cv2.waitKey(0)
cv2.destroyAllWindows
import cv2 as cv
import numpy as np
import imutils
from matplotlib import pyplot as plt
from flatten_json import flatten
from scipy.ndimage import median_filter
# A = [1:123]
img_path = 'C:/Users/ADMIN/Documents/VS_Code/Cpp_Image/picture10.jpg'
img = cv.imread(img_path)
img_gray = cv.imread(img_path, cv.IMREAD_GRAYSCALE)
rotate = imutils.rotate(img_gray, 3)
cropped_image = rotate[95:182, 355:780]
# cv.imshow('cropped_image', cropped_image)
brightness = 10 
contrast = 2.3 
image2 = cv.addWeighted(cropped_image, contrast, np.zeros(cropped_image.shape, rotate.dtype), 0, brightness)
# cv.imshow('image_his3', image2)

kernel2 = np.array([[0, -1, 0],
                [-1, 5,-1],
                [0, -1, 0]])
image_sharp = cv.filter2D(src=image2, ddepth=-1, kernel=kernel2)
# cv.imshow('image_his5', image_sharp)

blur1 = cv.GaussianBlur(image_sharp,(3, 3),0)
sharpe = cv.subtract(image2, blur1)
# cv.imshow('blur1', blur1)

cropped_image2 = image2[1:85, 93:162]
# cv.imshow('cropped_image2', cropped_image2)
erosion_img_name = 'picture_idetify1.jpg'
cv.imwrite(erosion_img_name, cropped_image2)

# img_blur = cv.blur(src=cropped_image2, ksize=(3,3)) # Using the blur function to blur an image where ksize is the kernel size
# hist,bins = np.histogram(img_blur.flatten(), 256, [0,256])
# cdf = hist.cumsum()
# cdf_normalized = cdf * float(hist.max())/cdf.max()
# cdf_m = np.ma.masked_equal(cdf, 0)
# cdf_m = (cdf_m - cdf_m.min())*255 / (cdf_m.max()-cdf_m.min())
# cdf = np.ma.filled(cdf_m, 0) .astype('uint8')
# img_his = cdf[img_blur]
    
# thresh, img_binary = cv.threshold(img_his, thresh=60, maxval=255, type=cv.THRESH_BINARY_INV)
# binary_img_name = 'picture_l_binary'+str(i+1)+'.jpg' 
# cv.imwrite(binary_img_name, img_binary)
    
# kernel = np.ones((3, 3), np.uint8)
# # img_erosion = cv.erode(img_binary, kernel, iterations=1)
# img_dilation = cv.dilate(img_binary, kernel, iterations=1)
# erosion_img_name = 'picture_dilation'+str(i+1)+'.jpg'
# cv.imwrite(erosion_img_name, img_dilation)
# cv.imshow('image_his1', img_his)
# cv.imshow('img_binary', img_binary)
# cv.imshow('img_erosion', img_erosion)
# cv.imshow('img_dilation', img_dilation)
cv.waitKey(0)
cv.destroyAllWindows()
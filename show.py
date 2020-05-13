import cv2
import matplotlib.pyplot as plt
im = cv2.imread('img_result/renkestu_out.png')
#im = cv2.resize(im,(im.shape[1]//5,im.shape[0]//5))
#cv2.imwrite('imgs/face.jpg', im)
plt.imshow(im[:,:,[2,1,0]])
plt.show()

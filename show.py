import cv2
import matplotlib.pyplot as plt
im = cv2.imread('imgs/football.jpg')
im = cv2.resize(im,(im.shape[1]//5,im.shape[0]//5))
cv2.imwrite('imgs/smallball.jpg', im)
plt.imshow(im[:,:,[2,1,0]])
plt.show()

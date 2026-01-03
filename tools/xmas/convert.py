import cv2

im = cv2.imread("xmas.png")

print(type(im))
# <class 'numpy.ndarray'>

print(im.shape)
# (225, 400, 3)

print(im.dtype)


def convTo01(x):
    if x[0] != 0:
        return "0"
    else:
        return "1"


for i in im:
    now = list(map(convTo01, i))
    print("".join(now))

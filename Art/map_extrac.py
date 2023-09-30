from PIL import Image
import numpy as np


# Size tail 32x32
TAIL_MAP = 'DoorstopDash/Tileset/row-1-column-2.png'
#TAIL_MAP = '45585.png'
MAP_DET = 'DoorstopDash/44917_bg.png'

map_tile = Image.open(TAIL_MAP)
map_det = Image.open(MAP_DET)

# Separing tail in set
def separing_tail(tailset):
    img_list = []
    image_arr = np.array(tailset)

    #print(f"Width {width} - Height {height}")
    # width dimension
    for j in range(0, int(tailset.height/32)):
        for i in range(0, int(tailset.width/32)):
            # width
            dim_w = 0+32*i
            dim_w_z = dim_w + 32
            # height
            dim_h = 0+32*j
            dim_h_z = dim_h + 32
            tail = image_arr[dim_h:dim_h_z, dim_w:dim_w_z] # 0:32, 0:32 [1]
            img_list.append(tail)
            #print(image_arr)
            #image = Image.fromarray(tail)
            #image.save(f"result/test_{j}_{i}.png")
        #return
    # print(img_list)
    return img_list

def indexOf(search, db):
    for i in db:
        print(f"Type {type(i)} - {type(search)}")
        if i == search:
            print(f"find in {i}")
    
database = separing_tail(map_tile)
map_img = separing_tail(map_det)

for j in map_img:
    indexOf(j, map_img)
    #break
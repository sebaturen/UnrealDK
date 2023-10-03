from PIL import Image
import numpy as np


# Size tail 32x32
TAIL_MAP = 'DoorstopDash/Tileset/row-1-column-2.png'
#TAIL_MAP = '45585.png'
MAP_DET = 'DoorstopDash/44917_bg.png'

map_tile = Image.open(TAIL_MAP)
map_det = Image.open(MAP_DET)

# Separing tail in set
def separing_tail(tailset, typed):
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
            #if j == 12 and i == 9 and typed == 2:
            #	return tail
            	#image = Image.fromarray(tail)
            	#image.save(f"result/test_{j}_{i}.png")
            #if j == 2 and i == 1 and typed == 1:
            #    return tail
                #image = Image.fromarray(tail)
                #image.save(f"result/db_{j}_{i}.png")
        #return
    # print(img_list)
    return img_list

def indexOf(search, db):
    post = 1
    for i in db:
        r = search - i
        r[(r >= 240) | (r <= 10)] = 0
        #print(f"Type {type(i)} - {type(search)}")
        if not np.any(r):
            if post == 1: # probably the error enable the empty tilset is equal to any other tile have only letters in black, so the information is a empty tile, force to really empty "0"
                return 0
            return post
        post += 1
    return 1 # have a information in map, not realy titl
    
database = separing_tail(map_tile, 1)
map_img = separing_tail(map_det, 2)

list_tiles = []
i = 1
for j in map_img:
    post = indexOf(j, database)
    list_tiles.append(post)
    if i == 81:
        print("")
        i = 1
    i+=1
    print(f"{post},", end="")
    #break
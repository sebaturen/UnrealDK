from PIL import Image
import numpy as np
import math


# Size tail 32x32
TAIL_MAP = 'DoorstopDash/Tileset/row-1-column-2.png'
LIST_TILE = [41,53,55,60,62,69,73,77,88,94,97,100,106,107,109,119,122,127,129,130,131,134,136,145,147,150,157,158,159,161,163,165,166,174,176,177,179,181,182,183,191,
             195,200,240,241,258,267,268,270,273,278,280,281,284,285,286,292,294,296,298,299,301,302,303,305,307,309,310,312,314,318,322,324,326,327,329,331,334,335,336,340,
             341,342,343,344,345,346,348,351,352,353,354,355,356,357,358,359,361,363,364,365,366,367,368,369,370,372,374,375,377,379,381,383,384,386,387,389,392,393,394,395,
             396,398,399,400,401,403,404,406,407,409,410,412,416,422,425,426,428,433,436,437,440,441,443,444,450,451,452,453,454,455,456,457,460,461,462,463,466,469,470,471,
             474,476,483,487,488,489,490,492,497,498,499,500,503,504,510,511]
map_tile = Image.open(TAIL_MAP)

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
    return img_list
    
def get_concat_h(im1, im2):
    dst = Image.new('RGBA', (im1.width + im2.width, im1.height))
    dst.paste(im1, (0, 0))
    dst.paste(im2, (im1.width, 0))
    return dst

def get_concat_v(im1, im2):
    dst = Image.new('RGBA', (im1.width, im1.height + im2.height))
    dst.paste(im1, (0, 0))
    dst.paste(im2, (0, im1.height))
    return dst

database = separing_tail(map_tile)

h_list = []
i, post = 0, 0
for j in LIST_TILE:
    process_image = Image.fromarray(database[j-1])
    if len(h_list) == post+1:
        h_list[post] = get_concat_h(h_list[post], process_image)
    else:
        h_list.append(process_image)
    if i == 16:
        i = 0
        post += 1
    else:
        i += 1
    
final_image = None
for i in h_list:
    if final_image:
        final_image = get_concat_v(final_image, i)
    else:
        final_image = i

final_image.save("DoorstopDash/Tileset/miss_generator.png")
from PIL import Image
import numpy as np


# Size tail 32x32
TAIL_MAP = 'DoorstopDash/Tileset/row-1-column-2.png'
LIST_TILE = [41,48,53,55,60,62,69,73,77,88,94,96,97,100,106,107,109,119,122,127,129,130,131,134,136,145,147,150,157,158,159,161,163,165,166,174,176,177,179,181,182,183,191,195,200,240,241,258,267,268,270,273,278,280,281,284,285,286,292,294,296,298,299,301,302,303,305,307,309,310,312,314,318,322,324,326,327,329,331,334,335,336,340,341,342,343,344,345,346,348,351,352,353,354,355,356,357,358,359,361,363,364,365,366,367,368,369,370,372,374,375,377,379,381,383,384,386,387,389,392,393,394,395,396,398,399,400,401,403,404,406,407,409,410,412,416,422,425,426,428,433,436,437,440,441,443,444,450,451,452,453,454,455,456,457,460,461,462,463,466,469,470,471,474,476,483,487,488,489,490,492,496,497,498,499,500,503,504,510,511]
map_tile = Image.open(TAIL_MAP)

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
    return img_list
    
database = separing_tail(map_tile, 1)

WORKING....
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
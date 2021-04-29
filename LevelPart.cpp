//#include "LevelPart.h"
//
//LevelPart::LevelPart(float p_x, float p_y) 
//    :x(p_x), y(p_y)
//{
//
//}
//
//bool LevelPart::setTiles(SDL_Texture* tileTex) {
//    //Success flag
//    bool tilesLoaded = true;
//
//    //The tile offsets
//    int x = getX(), y = getY();
//
//    //Mở map
//    ifstream map("res/texture/map.map");
//
//    //Nếu ko đọc đc dữ liệu trong map
//    if (map.fail()) {
//        printf("Unable to load map file!\n");
//        tilesLoaded = false;
//    }
//    else
//    {
//        //Tạo các tile
//        for (int i = 0; i < TOTAL_TILES; ++i) {
//            //Chọn loại cho tile
//            int tileType = -1;
//
//            //Đọc từ map
//            map >> tileType;
//
//            //Debug
//            if (map.fail()) {
//                printf("Error loading map: Unexpected end of file!\n");
//                tilesLoaded = false;
//                break;
//            }
//
//            //Nếu như đọc đc tileType và thỏa mãn
//            if ((tileType >= 0) && (tileType < TOTAL_TILE_SPRITES)) {
//                tiles[i] = new Tile(x, y, tileTex, tileType);
//            }
//            //Nếu như tileType ko thoả mãn
//            else
//            {
//                printf("Error loading map: Invalid tile type at %d!\n", i);
//                tilesLoaded = false;
//                break;
//            }
//            //Dịch x của tile tiếp theo
//            x += TILE_WIDTH;
//
//            //Nếu như đến giới hạn level
//            if (x >= LEVEL_WIDTH) {
//                //Xuống dòng mới và làm lại
//                x = 0;
//                y += TILE_HEIGHT;
//            }
//        }
//
//        if (tilesLoaded)
//        {
//            int m = 0, n = 0;
//            for (int i = 0; i < TOTAL_TILE_SPRITES; i++) {
//                gTileClips[i].x = n;
//                gTileClips[i].y = m;
//                gTileClips[i].w = TILE_WIDTH;
//                gTileClips[i].h = TILE_HEIGHT;
//                n += TILE_WIDTH;
//                if (n >= 17 * TILE_WIDTH) {
//                    n = 0;
//                    m += TILE_HEIGHT;
//                }
//            }
//        }
//    }
//
//    //Close the file
//    map.close();
//
//    //If the map was loaded fine
//    return tilesLoaded;
//}
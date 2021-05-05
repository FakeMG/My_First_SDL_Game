# MyGame
- Các thư viện SDL: SDL.h, SDL_image.h, SDL_mixer.h, SDL_TTF.h
- Code tham khảo: 
	+ Toàn bộ 2 file Time.h và Time.cpp: https://lazyfoo.net/tutorials/SDL/23_advanced_timers/index.php
	+ Hàm createText(): https://lazyfoo.net/tutorials/SDL/16_true_type_fonts/index.php
	+ Hàm checkCollision(): https://lazyfoo.net/tutorials/SDL/27_collision_detection/index.php
- Giới thiệu game: Game kết hợp 2 thể loại platformer (VD: Mario, contra,...) và endless runner (VD: Temple run,...). 
- Nội dung: Bạn vào vai 1 kị sĩ đang ở trong ngục tối, bạn cần phải thoát ra trước khi ngục tù này bị phá hủy. Trên đường thoát ra sẽ có những con quái vật xương cản đường bạn, bạn có thể bắn chúng bằng súng được trang bị trên người hoặc tìm cách nhảy qua chúng.
- Các tính năng của game: 
	+ Có hàm tạo ra level ngẫu nhiên, vô tận từ 15 level được tạo sẵn.
	+ Các con quái vật biết tự di chuyển và sẽ tấn công người chơi khi người chơi ở trong tầm nhìn.
	+ Người chơi và quái vật có các animation riêng cho từng trạng thái chuyển động (chạy, nhảy, rơi,...).
	+ Có nhạc và hiệu ứng âm thanh cho các hành động của người chơi và quái vật.
	+ Game sẽ tính điểm dựa trên quãng đường người chơi đi được.
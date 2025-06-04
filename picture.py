import sys
import pygame

pygame.init()

# Lấy tham số từ dòng lệnh
nguon = sys.argv[1]
dich = sys.argv[2]

# Ví dụ: ánh xạ cặp thành ảnh
if (nguon == "TC" and dich == "D3") or (nguon == "D3" and dich == "TC"):
    image_file = "TCdenD3.png"
elif (nguon == "B1" and dich == "D3-5") or (nguon == "D3-5" and dich == "B1"):
    image_file = "B1denD3-5.png"
else:
    print("Không tìm thấy ảnh phù hợp.")
    sys.exit()

# Hiển thị ảnh
image = pygame.image.load(image_file)
window_size = image.get_size()
screen = pygame.display.set_mode(window_size)
pygame.display.set_caption(f"Hiển thị ảnh: {image_file}")

running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        if event.type == pygame.KEYDOWN and event.key == pygame.K_RETURN:
            running = False

    screen.fill((0, 0, 0))
    screen.blit(image, (0, 0))
    pygame.display.flip()

pygame.quit()
sys.exit()

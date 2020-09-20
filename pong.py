import json
import pygame
import math

with open('/Users/togeka/Desktop/speed.json') as f:
	data = json.load(f)


#make game screen
wScreen = 1200
hScreen = 500

bg = pygame.image.load("bg.png")

#INSIDE OF THE GAME LOOP
gameDisplay.blit(bg, (0, 0))

win = pygame.display.set_mode((wScreen, hScreen))


class Ball(pygame.sprite.Sprite):
	def __init__(self, x, y, radius, color):
		super().__init__()
		#starts out on the ground
		self.x = x
		self.y = y
		self.radius = radius
		self.color = color
		#starts out zero before we throw 
		self.x_velocity = 0
		self.y_velocity = 0
		self.quit = False

	def draw(self, win):
		pygame.draw.circle(win, (0,0,0), (self.x,self.y), self.radius)
		pygame.draw.circle(win, self.color, (self.x,self.y), self.radius-1)

	def ball_move(self, x_velocity, y_velocity, time):
		self.x = x + int(x_velocity*time)
		self.y = y + int(-1*y_velocity*time - 0.5*a_y*time**2)
		if self.y > 500:
			self.quit = True
		print(self.x, self.y)

		


def redrawWindow():
	win.fill((64,64,64))
	pong_ball.draw(win)
	pygame.display.update()



a_y = -9.81
time = 0
x_velocity = data['x_velocity']
y_velocity = data['y_velocity']
x = 100
y = 300



pong_ball = Ball(300,494,5,(255,255,255))
run = True

while run:
	pong_ball.ball_move(x_velocity, y_velocity, time)
	time += .1
	if pong_ball.quit:
		break

	redrawWindow()
	for event in pygame.event.get():
		
		if event.type == pygame.QUIT:
			run = False

pygame.quit()






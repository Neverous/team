import sys
from PIL import Image
im = Image.open(sys.argv[1])
horiz = [[0] * im.size[1] for j in range(im.size[0])]
vertical = [[0] * im.size[1] for j in range(im.size[0])]
diagonalA = [[0] * im.size[1] for j in range(im.size[0])]
diagonalB = [[0] * im.size[1] for j in range(im.size[0])]

for i in range(im.size[0]):
	for j in range(im.size[1]):
		horiz[i][j] = (i - 1 >= 0 and horiz[i - 1][j]) + (im.getpixel((i, j)) == 0)
		vertical[i][j] = (j - 1 >= 0 and vertical[i][j - 1]) + (im.getpixel((i, j)) == 0)
		diagonalA[i][j] = (i - 1 >= 0 and j - 1 >= 0 and diagonalA[i - 1][j - 1]) + (im.getpixel((i, j)) == 0)
		diagonalB[i][j] = (i - 1 >= 0 and j + 1 < im.size[1] and diagonalB[i - 1][j + 1]) + (im.getpixel((i, j)) == 0)

def checkLine(A, B):
	if A[0] == B[0]:
		return abs(vertical[A[0]][A[1]] - vertical[B[0]][B[1]]) == abs(A[1] - B[1])

	if A[1] == B[1]:
		return abs(horiz[A[0]][A[1]] - horiz[B[0]][B[1]]) == abs(A[0] - B[0])

def checkDiagonal(A, B):
	dx = -1 if A[0] > B[0] else 1
	dy = -1 if A[1] > B[1] else 1
	if dx < 0:
		if dy < 0:
			return abs(diagonalA[A[0]][A[1]] - diagonalA[B[0]][B[1]]) == abs(A[0] - B[0])

		else:
			return abs(diagonalB[A[0]][A[1]] - diagonalB[B[0]][B[1]]) == abs(A[0] - B[0])

	else:
		if dy < 0:
			return abs(diagonalB[A[0]][A[1]] - diagonalB[B[0]][B[1]]) == abs(A[0] - B[0])

		else:
			return abs(diagonalA[A[0]][A[1]] - diagonalA[B[0]][B[1]]) == abs(A[0] - B[0])

count = 0
for dx in range(-1, 2, 2):
	for dy in range(-1, 2, 2):
		for x in range(im.size[0]):
			for y in range(im.size[1]):
				if im.getpixel((x, y)):
					continue

				for r in range(1, min(im.size[0], im.size[1])):
					if not 0 <= x + r*dx < im.size[0] or not 0 <= y + r*dy < im.size[1]:
						break

					if im.getpixel((x + r*dx, y)) or im.getpixel((x, y + r*dy)):
						break

					count += checkDiagonal((x + r*dx, y), (x, y + r*dy))

for x in range(im.size[0]):
	for y in range(im.size[1]):
		if im.getpixel((x, y)):
			continue

		for r in range(1, im.size[0]):
			if not 0 <= x + r < im.size[0] or not 0 <= y + r < im.size[1] or not 0 <= y - r < im.size[1]:
				break

			if im.getpixel((x + r, y - r)) or im.getpixel((x + r, y + r)):
				break

			count += checkLine((x + r, y - r), (x + r, y + r))

		for r in range(1, im.size[0]):
			if not 0 <= x - r < im.size[0] or not 0 <= y + r < im.size[1] or not 0 <= y - r < im.size[1]:
				break

			if im.getpixel((x - r, y - r)) or im.getpixel((x - r, y + r)):
				break

			count += checkLine((x - r, y - r), (x - r, y + r))

		for r in range(1, im.size[1]):
			if not 0 <= y - r < im.size[1] or not 0 <= x + r < im.size[0] or not 0 <= x - r < im.size[0]:
				break

			if im.getpixel((x + r, y - r)) or im.getpixel((x - r, y - r)):
				break

			count += checkLine((x + r, y - r), (x - r, y - r))

		for r in range(1, im.size[1]):
			if not 0 <= y + r < im.size[1] or not 0 <= x + r < im.size[0] or not 0 <= x - r < im.size[0]:
				break

			if im.getpixel((x + r, y + r)) or im.getpixel((x - r, y + r)):
				break

			count += checkLine((x + r, y + r), (x - r, y + r))

print count

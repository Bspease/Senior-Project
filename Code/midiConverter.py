
song = "00 90 40 32 60 80 40 00 81 40 90 40 32 60 80 40 00 81 40 90 40 32 60 80 40 00 83 60 90 40 32 60 80 40 00 81 40 90 40 32 60 80 40 00 81 40 90 40 32 60 80 40 00 83 60 90 40 32 60 80 40 00 81 40 90 43 32 60 80 43 00 81 40 90 3C 32 60 80 3C 00 81 40 90 3E 32 60 80 3E 00 81 40 90 40 32 60 80 40 00 88 20 90 41 32 60 80 41 00 81 40 90 41 32 60 80 41 00 81 40 90 41 32 60 80 41 00 83 60 90 41 32 60 80 41 00 81 40 90 40 32 60 80 40 00 81 40 90 40 32 60 80 40 00 83 60 90 40 32 60 80 40 00 81 40 90 3E 32 60 80 3E 00 81 40 90 3E 32 60 80 3E 00 81 40 90 3C 32 60 80 3C 00 81 40 90 3E 32 60 80 3E 00 83 60 90 43 32 60 80 43 00"

def main():
   out = []
   out2 = []
   new = 0
   turn = 0

   songArr = song.split()
   for h in songArr:
      h = int(h,16)
      out.append(h)
      #delay
      if turn == 0:
         if h & 0x80:
            continue
      elif turn == 1:
         if h == 0x90:
            new = 1
         elif h == 0x80:
            new = 0
         else:
            print("ERROR: Format Issue or more than one track: " + h)
            return
      elif turn == 2:
            if new == 1:
               out2.append(h)
      elif turn == 3:
         turn = -1
      turn += 1
   print(out)
   print('*')
   print("Notes:")
   print(out2)

if __name__ == "__main__":
   main()

#!/bin/python3

def showInstructions():
    print('''
RPG Game
========

Consigue llegar al jardin con la llave y una posion
Evita a los monstruos!... a menos que tengas con que defenderte

Comandos:
  go [direccion (south/north/east/west)]
  get [objeto (key/potion/gun)]
''')


spanishNames = {
    'Hall': 'Sala',
    'Dining Room': 'Comedor',
    'Garden': 'Jardin',
    'potion': 'posion',
    'key': 'llave',
    'Kitchen': 'Cocina',
    'Beadroom': 'Dormitorio',
    'Bathroom': 'Baño',
    'gun': 'pistola'
}

def showStatus():
  spanishRoom = spanishNames[currentRoom]
  #print the player's current status
  print('--------------------------------')
  print('Estas en ' + spanishRoom)
  #print the current inventory
  print("Inventario : " + str(inventory))
  #print an item if there is one
  if "item" in rooms[currentRoom]:
    print('Tu vez una ' + spanishNames[rooms[currentRoom]['item']])
  
  print('-------------------------------')

#an inventory, which is initially empty
inventory = []

#a dictionary linking a room to other room positions
rooms = {

            'Hall' : { 
                  'south' : 'Kitchen',
                  'east'  : 'Dining Room',
                  'west'  : 'Beadroom',
                  'item'  : 'key'
                },        

            'Kitchen' : { 
                  'west': 'Bathroom',
                  'north': 'Hall',
                  'item'  : 'monster'
                },
                
            'Dining Room' : { 
                  'west'  : 'Hall',
                  'south' : 'Garden',
                  'item'  : 'potion'
              
                },
            'Beadroom': {
                'east': 'Hall',
                'south': 'Bathroom',
                'item'  : 'gun',
            },
            'Bathroom': {
              'east': 'Kitchen',
              'north': 'Beadroom',
            },
            'Garden' : { 'north' : 'Dining Room' }

         }

#start the player in the Hall
currentRoom = 'Hall'

showInstructions()

#loop forever
while True:

  showStatus()

  #get the player's next 'move'
  #.split() breaks it up into an list array
  #eg typing 'go east' would give the list:
  #['go','east']
  move = ''
  while move == '':  
    move = input('>')
    
  move = move.lower().split()

  #if they type 'go' first
  if move[0] == 'go':
    #check that they are allowed wherever they want to go
    if move[1] in rooms[currentRoom]:
      #set the current room to the new room
      currentRoom = rooms[currentRoom][move[1]]
    #there is no door (link) to the new room
    else:
      print('No puedes ir en esa direccion')

  #if they type 'get' first
  if move[0] == 'get' :
    spanishItem = spanishNames[move[1]]
    #if the room contains an item, and the item is the one they want to get
    if 'item' in rooms[currentRoom] and move[1] in rooms[currentRoom]['item']:
      #add the item to their inventory
      inventory += [move[1]]
      #display a helpful message
      print('Obtuviste una ' + spanishItem)
      #delete the item from the room
      del rooms[currentRoom]['item']
    #otherwise, if the item isn't there to get
    else:
      #tell them they can't get it
      print('No puedes tomar la ' + spanishItem + '!')

  # player loses if they enter a room with a monster
  if 'item' in rooms[currentRoom] and 'monster' in rooms[currentRoom]['item']:
    if 'gun' in inventory: 
      print('Te has topado con un monstruo!, pero has reaccionado rapido y lo has matado')
      del rooms[currentRoom]['item']
    else:
      print('Un monstruo te ha encontrado, HAS PERDIDO!')
      break
  # player wins if they get to the garden with a key and a potion
  if currentRoom == 'Garden':
    if 'key' in inventory and 'potion' in inventory:  
      print('Has escapado de la casa, HAS GANADO!')
      break
    else: print("No puedes terminar el juego si no has recolectado los objetos")
  
# bot.py
import os

import discord
import serial
import time

from discord.ext import commands
from dotenv import load_dotenv

intents = discord.Intents.default()
intents.message_content = True
intents.members = True
intents.presences = True

# loads bot token from .env file
load_dotenv()
TOKEN = os.getenv('BOT_TOKEN')

# sets up discord bot client and also bot that takes commands
bot = commands.Bot(command_prefix='/', intents=intents)

# Open the serial port
ser = serial.Serial('/dev/cu.usbserial-14310', 9600)

# # prints on successful connect
@bot.event
async def on_ready():
    print(f'{bot.user.name} has connected to Discord!')

# welcomes new members
@bot.event
async def on_member_join(member):
    print(f'new member')
    channel = member.guild.system_channel
    if channel is not None:
        await channel.send(f'Welcome to the server, {member.mention}!')

# # repeats message received
# @bot.event
# async def on_message(msg):
#     if msg.author == bot.user:
#         return

#     await msg.channel.send(f'received: {msg.content}')

# echos back user input message
@bot.command()
async def echo(ctx, arg):
    print('echoing')
    await ctx.send(arg)

# listing commands
@bot.command()
async def lst(ctx):
    print('listing commands')
    comm = {"lst":"returns list of commands",
            "getTemp":"returns the current temperature of drink",
            "tempRecs":"returns a list of recommended drink temperatures",
            "setTemp":"sets drink to specified temperature, example: /setTemp 82",
            "makeHot":"sets drink to recommended hot temperature (136 F)",
            "makeCold":"sets drink to recommended cold temperature (44 F)"
            }
    await ctx.send(f'list of commands and descriptions:\n {comm}')

# read arduino serial port
@bot.command()
async def getTemp(ctx):
    print('getting current temp')
    data = ser.read(5)
    data = str(data, encoding='utf-8')
    await ctx.send(f'Temperature data: {data}')

#tempRec command (print temp recs for hot, cold, and room temp drink)
@bot.command()
async def tempRecs(ctx):
    print('getting temp recs')
    hot_recs = "hot drinks: 125-136 Farenheit\n"
    cold_recs = "cold drinks: 44-50 Farenheit\n"
    room_recs = "room temp drinks: 68-74 Farenheit\n"
    await ctx.send(f'Temperature recomendations:\n {hot_recs,cold_recs,room_recs}')

#setTemp command (let user set whatever temp BUT have a max and min so people cant set to 400 or something)
@bot.command()
async def setTemp(ctx, arg):
    print('setting temp')
    data = str(arg)
    ser.write(arg.encode())
    if "." in data:
        await ctx.send(f'Please enter temperature as an Integer. Ex: 60')
    elif int(arg) > 136 or int(arg) < 44:
          await ctx.send(f'The temp u entered is not recommended for consumption! Please choose a temp between 44-136 Farenheit or use the command /tempRecs for drinking temperature recommendations.')      
    else:
        await ctx.send(f'Setting temp to: {data}')
        status = ser.read(5)
        status = str(status, encoding='utf-8')

@bot.command()
async def makeHot(ctx):
    print('making hotter')
    ser.write('makeHot'.encode())

@bot.command()
async def makeCold(ctx):
    print('making colder')
    ser.write('makeCold'.encode())

# changes the temperature of your drink based on weather
@bot.command()
async def weather(ctx, arg):
    # we want this command to change the temperature of a drink based on the temperature outside
    # the user should enter in the temperature outside, and the peltier should change temperatures 
    # for example, if it is hot outside, the drink should be set to a cold temp and vice versa
    # so we want this command to prompt the user for the temp
    print()
    
# asks the user how they are feeling
@bot.command()
async def feeling(ctx, arg):
    # so we want this command to prompt the user for how they are feeling like sad, sick, happy, angry
    # also want this to change the temp based on mood
    print()

# send data to serial port
@bot.command()
async def send(ctx, arg):
    print('sending data')
    data = int(arg)
    ser.write(data)
    await ctx.send(f'Sent data: {data}')

bot.run(TOKEN)
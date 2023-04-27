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
ser = serial.Serial('/dev/cu.usbserial-14410', 9600)

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

# read arduino serial port
@bot.command()
async def getTemp(ctx):
    print('getting current temp')
    # ser.write('getTemp'.encode())
    # data = ser.readline().decode().rstrip()
    data = ser.read(5)
    data = str(data, encoding='utf-8')
    await ctx.send(f'Temperature data: {data}')

#TODO
#tempRec command (print temp recs for hot, cold, and room temp drink)
#setTemp command (let user set whatever temp BUT have a max and min so people cant set to 400 or something)

@bot.command()
async def makeHotter(ctx):
    print('making hotter')
    ser.write('makeHotter'.encode())

# send data to serial port
@bot.command()
async def send(ctx, arg):
    print('sending data')
    data = int(arg)
    ser.write(data)
    await ctx.send(f'Sent data: {data}')

# light test
@bot.command()
async def light(ctx, arg):
    if arg.lower() == "on":
        print("The LED is on...")
        time.sleep(1) 
        ser.write('ON'.encode())
    elif arg.lower() == "off":
        print("The LED is off...")
        time.sleep(1) 
        ser.write('OFF'.encode())
    await ctx.send(f'Light status: {arg}')

bot.run(TOKEN)
# bot.py
import os

import discord
import serial

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
ser = serial.Serial('COM3', 115200)

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
async def readTemp(ctx):
    print('reading current temp')
    data = ser.readline().decode().rstrip()
    await ctx.send(f'Received data: {data}')

# send data to serial port
@bot.command()
async def send(ctx, arg):
    print('sending data')
    data = int(arg)
    ser.write(data)
    await ctx.send(f'Sent data: {data}')

bot.run(TOKEN)
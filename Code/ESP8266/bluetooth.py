import sys
import asyncio
import datetime as dt

import serial_asyncio
import aioconsole

import urllib.request

url = "http://192.168.1.24/"

# Get data from the ip address
async def get_data():
    n = urllib.request.urlopen(url).read()
    n = n.decode("utf-8")
    return n

# Send a message to the Arduino using the serial connection
async def send_message(writer, message):
    writer.write(message.encode())
    await writer.drain()


# Read a message from the Arduino using the serial connection
async def read_message(reader):
    data = await reader.readuntil(b'\n')
    return data.decode()


# Main function

async def main():
    # Establish the serial connection
    reader, writer = await serial_asyncio.open_serial_connection(url='COM10', baudrate=9600)
    # read message from ip address and send it to arduino via serial
    while True:
        data = await get_data()
        #convert data to char from ascii code
        msg = data
        print(msg)
        await send_message(writer, msg + '\n')
        await asyncio.sleep(0.1)

if __name__ == '__main__':
    asyncio.run(main())

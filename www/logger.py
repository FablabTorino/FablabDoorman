#!/usr/bin/env python

from sys import argv
import sqlite3
from datetime import datetime
db = sqlite3.connect('logger.db')
c = db.cursor()

c.execute('''CREATE TABLE IF NOT EXISTS intothedoor(datetime text, cardcode text)''')

date = str(datetime.utcnow().isoformat())

filename=open('log.txt', 'w')

cardcode = ''.join(argv[1:])
c.execute("INSERT INTO intothedoor(datetime, cardcode) VALUES (?,?)", (date, cardcode))

db.commit()

filename.write(cardcode)
filename.close()


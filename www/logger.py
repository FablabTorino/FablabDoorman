#!/usr/bin/env python

from sys import argv
import sqlite3
from datetime import datetime
import os
source_dir = os.path.dirname(os.path.abspath(__file__))
cardcode = ''.join(argv[1])
db = sqlite3.connect(os.path.join(source_dir, 'logger.db'))
c = db.cursor()

c.execute('''CREATE TABLE IF NOT EXISTS intothedoor(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, datetime TEXT NOT NULL, cardcode TEXT NOT NULL)''')
c.execute('''CREATE TABLE IF NOT EXISTS allowedusers(id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL, username TEXT NOT NULL, cardcode TEXT NOT NULL)''')

def insert_user(dataBase, username=None, cardcode=None):
  c = db.cursor()
  log = open('log.txt', 'w')
  if username and cardcode:
		c.execute("INSERT INTO allowedusers(username, cardcode) VALUES (?,?)", (username, cardcode))
		dataBase.commit()
  else:
		log.write('You need to provide a username and a cardcode')
		return
  log.close()

  return

#insert_user(db, 'Federico Vanzati', '1815453204')

def log_rfid_read(dataBase, cardcode):
  c = db.cursor()
  date = str(datetime.utcnow().isoformat())
  c.execute("INSERT INTO intothedoor(datetime, cardcode) VALUES (?,?)", (date, cardcode))
  dataBase.commit()
  return

def check_allowed_user(dataBase, cardcode=None):
  c = db.cursor()
  if cardcode:
		cardcode = (cardcode,)
		c.execute("select count (*) from allowedusers where cardcode=?", cardcode)
		result = c.fetchone()[0]
		
		if result > 0:
			print 'y'
			return
		else:
			print 'n'

  return

log_rfid_read(db, cardcode)
check_allowed_user(db, cardcode)

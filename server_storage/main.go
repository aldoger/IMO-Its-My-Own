package main

import (
	"imo-server-storage/src/config"
	"imo-server-storage/src/db"
)

func main() {
	database := db.New()

	eng := config.Config(database)
	eng.Run(":8080")
}

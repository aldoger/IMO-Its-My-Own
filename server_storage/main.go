package main

import (
	"imo-server-storage/src/config"
)

func main() {
	// TODO: initialize *gorm.DB here (e.g. gorm.Open with your driver)
	eng := config.Config(nil)
	eng.Run(":8080")
}

package config

import (
	"imo-server-storage/src/controller"
	"imo-server-storage/src/repo"
	"imo-server-storage/src/routes"
	"imo-server-storage/src/service"

	"github.com/gin-gonic/gin"
	"gorm.io/gorm"
)

func Config(db *gorm.DB) *gin.Engine {
	eng := gin.Default()

	userRepo := repo.NewUserRepo(db)
	userService := service.NewUserService(userRepo)
	userController := controller.NewUserController(userService)

	routes.Hello(eng)
	routes.User(eng, userController)

	return eng
}

package controller

import (
	"imo-server-storage/src/dto"
	"imo-server-storage/src/service"

	"github.com/gin-gonic/gin"
)

type UserController struct {
	userService *service.UserService
}

func NewUserController(userService *service.UserService) UserController {
	return UserController{userService: userService}
}

func (uc *UserController) CreateNewUser(c *gin.Context) {
	var req dto.CreateUserRequest
	if err := c.ShouldBindBodyWithJSON(&req); err != nil {
		c.JSON(400, gin.H{"error": "invalid request"})
		return
	}

	createUser, err := uc.userService.CreateUser(c, req)
	if err != nil {
		c.JSON(500, gin.H{"error": "internal server error"})
		return
	}

	c.JSON(201, gin.H{"data": createUser})
}

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

func (c *UserController) CreateNewUser(ctx *gin.Context) {
	var req dto.CreateUserRequest
	if err := ctx.ShouldBindBodyWithJSON(&req); err != nil {
		ctx.JSON(400, gin.H{"error": "invalid request"})
		return
	}

	createUser, err := c.userService.CreateUser(ctx, req)
	if err != nil {
		ctx.JSON(500, gin.H{"error": "internal server error"})
		return
	}

	ctx.JSON(201, gin.H{"data": createUser})
}

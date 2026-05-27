package routes

import (
	"imo-server-storage/src/controller"

	"github.com/gin-gonic/gin"
)

func User(c *gin.Engine, userController controller.UserController) {
	routes := c.Group("/user")
	{
		routes.POST("/register", userController.CreateNewUser)
	}
}

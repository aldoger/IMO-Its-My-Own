package routes

import "github.com/gin-gonic/gin"

func Hello(c *gin.Engine) {
	routes := c.Group("/hello")
	{
		routes.GET("", func(ctx *gin.Context) {
			ctx.JSON(200, gin.H{"data": "Hello from IMO server"})
		})
	}
}

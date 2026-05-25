package config

import "github.com/gin-gonic/gin"

func Config() *gin.Engine {
	eng := gin.Default()

	return eng
}

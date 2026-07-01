package controller

import (
	"imo-server-storage/src/dto"
	"imo-server-storage/src/service"
	"imo-server-storage/src/utils"

	"github.com/gin-gonic/gin"
)

type ShareController struct {
	shareService *service.ShareService
}

func NewShareController(shareService *service.ShareService) ShareController {
	return ShareController{shareService: shareService}
}

func (c *ShareController) CreateShare(ctx *gin.Context) {

	var req dto.CreateShareRequest
	if err := ctx.ShouldBind(&req); err != nil {
		ctx.JSON(400, gin.H{"error": utils.ErrBadRequest})
		return
	}

	if err := c.shareService.CreateShare(ctx, req); err != nil {
		ctx.JSON(500, gin.H{"error": err.Error()})
		return
	}

	ctx.JSON(201, gin.H{"message": "file shared success"})
}

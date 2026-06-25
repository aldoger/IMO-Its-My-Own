package controller

import (
	"imo-server-storage/src/dto"
	"imo-server-storage/src/service"
	"imo-server-storage/src/utils"

	"github.com/gin-gonic/gin"
)

type FileController struct {
	fileService service.FileService
}

func NewFileController(fileService *service.FileService) FileController {
	return FileController{fileService: *fileService}
}

func (c *FileController) UploadFile(ctx *gin.Context) {
	var req dto.UploadFileRequest
	if err := ctx.ShouldBind(&req); err != nil {
		ctx.JSON(400, gin.H{"error": utils.ErrBadRequest})
		return
	}

	err := c.fileService.Upload(ctx, &req)
	if err != nil {
		ctx.JSON(500, gin.H{"error": err.Error()})
		return
	}

	ctx.JSON(201, gin.H{"message": "file uploaded"})
}

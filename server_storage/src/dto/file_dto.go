package dto

import "mime/multipart"

type UploadFileRequest struct {
	UserID   string          `form:"user_id"`
	FileName string          `form:"file_name"`
	File     *multipart.File `form:"file"`
}

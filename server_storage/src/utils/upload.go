package utils

import (
	"context"
	"io"
	"mime/multipart"
	"os"
	"path/filepath"
)

func UploadFile(ctx context.Context, file multipart.File, fileName string) error {
	if err := os.MkdirAll("store", 0755); err != nil {
		return err
	}

	dst, err := os.Create(filepath.Join("store", fileName))
	if err != nil {
		return err
	}
	defer dst.Close()

	_, err = io.Copy(dst, file)
	return err
}

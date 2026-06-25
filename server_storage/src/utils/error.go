package utils

import "errors"

var (
	ErrNotFound   = errors.New("resource not found")
	ErrBadRequest = errors.New("bad request")
)

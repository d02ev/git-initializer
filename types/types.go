package types

type Config struct {
	Key      string `json:"key"`
  Name     string `json:"name"`
  FileName string `json:"fileName"`
  Contents string `json:"contents"`
}

type IgnoresJsonConfig map[string]Config;
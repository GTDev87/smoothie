defmodule SmoothieApi.Web.Repo do
  use Ecto.Repo, otp_app: :smoothie_api, adapter: Ecto.Adapters.Postgres
  require Logger
  def init(_type, config) do
    url = Keyword.get(config, :url)

    options = DatabaseUrl.parse(url)

    new_config =
      config 
      |> Keyword.put(:username, Keyword.get(options, :username))
      |> Keyword.put(:password, Keyword.get(options, :password))
      |> Keyword.put(:hostname, Keyword.get(options, :host))
      |> Keyword.put(:database, Keyword.get(options, :database))
      |> Keyword.delete(:url)
    
    {:ok, new_config}
  end
end

defmodule SmoothieApi.Web.WriteRepo do
  use Ecto.Repo, otp_app: :smoothie_api, adapter: Ecto.Adapters.Postgres
  require Logger
  def init(_type, config) do
    url = Keyword.get(config, :url)

    options = DatabaseUrl.parse(url)

    new_config =
      config 
      |> Keyword.put(:username, Keyword.get(options, :username))
      |> Keyword.put(:password, Keyword.get(options, :password))
      |> Keyword.put(:hostname, Keyword.get(options, :host))
      |> Keyword.put(:database, Keyword.get(options, :database))
      |> Keyword.delete(:url)
    
    {:ok, new_config}
  end
end

defmodule SmoothieApi.Web.ReadWriteRepo do
  require Logger

  SmoothieApi.Web.Repo.__info__(:functions)
  |> Enum.each(fn {func_name, arity} ->
    fn_args = SmoothieApi.Web.Lib.RepoUtils.create_args(SmoothieApi.Web.Repo, arity)

    all_fn_args =
      [Macro.var(:type, nil)] ++
        SmoothieApi.Web.Lib.RepoUtils.create_args(SmoothieApi.Web.Repo, arity)

    def unquote(:"#{func_name}")(unquote_splicing(all_fn_args)) do
      case type do
        :mutation -> SmoothieApi.Web.WriteRepo
        _ -> SmoothieApi.Web.Repo
      end
      |> apply(unquote(func_name), unquote(fn_args))
    end
  end)
end

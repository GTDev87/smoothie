defmodule AssessmentApi.Web.Repo do
  use Ecto.Repo, otp_app: :assessment_api
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

defmodule AssessmentApi.Web.WriteRepo do
  use Ecto.Repo, otp_app: :assessment_api
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

defmodule AssessmentApi.Web.ReadWriteRepo do
  require Logger

  AssessmentApi.Web.Repo.__info__(:functions)
  |> Enum.each(fn {func_name, arity} ->
    fn_args = AssessmentApi.Web.Lib.RepoUtils.create_args(AssessmentApi.Web.Repo, arity)

    all_fn_args =
      [Macro.var(:type, nil)] ++
        AssessmentApi.Web.Lib.RepoUtils.create_args(AssessmentApi.Web.Repo, arity)

    def unquote(:"#{func_name}")(unquote_splicing(all_fn_args)) do
      case type do
        :mutation -> AssessmentApi.Web.WriteRepo
        _ -> AssessmentApi.Web.Repo
      end
      |> apply(unquote(func_name), unquote(fn_args))
    end
  end)
end

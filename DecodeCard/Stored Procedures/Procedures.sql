--����pr_ErrorRecords �����ڵ�ʱ��
if not exists (select * from dbo.sysobjects where id = object_id(N'[dbo].[pr_ErrorRecords]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
begin
exec('CREATE PROCEDURE pr_ErrorRecords
(
	@ZKZM_Number varchar(50),--׼��֤�����
	@Ejlbh bigint, --�����¼���
	@intErrorCode int OUTPUT
) 
WITH   ENCRYPTION 
AS
BEGIN
SET NOCOUNT ON --�����ؼ���
set @intErrorCode=-1
return @intErrorCode
END
')
end
GO
--�޸� pr_ErrorRecords 2014-10-09 �ӵ��մ����ж� 
ALTER PROCEDURE pr_ErrorRecords
(
	@ZKZM_Number varchar(50),--׼��֤�����
	@Ejlbh bigint, --�����¼���
	@intErrorCode int OUTPUT
) 
WITH   ENCRYPTION 
AS
BEGIN
SET NOCOUNT ON --�����ؼ���
DECLARE @Status char(1)--״̬
declare @KscsN tinyint --���Դ���
declare @DrcsN tinyint --���մ���
declare @KSCJ int --�ɼ�
declare @ErrorCode int --������
declare @ErrorCodejl int --��¼���
SELECT @Status=״̬,@KscsN=���Դ���,@DrcsN=���մ��� FROM Studentinfo WHERE ׼��֤�����=@ZKZM_Number
SELECT @KSCJ=100-SUM(�۳�����) FROM ErrorRecords JOIN ErrorData ON  ErrorRecords.������=ErrorData.������ 
WHERE ErrorRecords.׼��֤�����=@ZKZM_Number and ���Դ���=@KscsN and ���մ���=@DrcsN

set @intErrorCode = 0
if exists(Select TOP 1 ������ from ErrorRecords where ׼��֤�����=@ZKZM_Number and ��¼���>@Ejlbh and ���մ���=@DrcsN and ���Դ���=@KscsN)
begin --1b
Select TOP 1 @ErrorCode=������,@ErrorCodejl=��¼��� from ErrorRecords where ׼��֤�����=@ZKZM_Number and ��¼���>@Ejlbh and ���մ���=@DrcsN and ���Դ���=@KscsN
if exists(select * from errordata where �۳�����!=0 and ������=@ErrorCode) 
BEGIN --2b
declare @counterrline int --ͳ�ƴ�����
SELECT @counterrline=Count(*) FROM ErrorRecords JOIN ErrorData ON  ErrorRecords.������=ErrorData.������ 
WHERE ErrorRecords.׼��֤�����=@ZKZM_Number and ���Դ���=@KscsN and ���մ���=@DrcsN and ErrorData.�۳�����!=0 and ErrorRecords.��¼���<=@Ejlbh

Select ErrorRecords.��¼���,ErrorData.�۷�����,CONVERT(varchar(10),ErrorRecords.����ʱ��,8) as ����ʱ��,ErrorData.�۳�����,�ɼ�=@KSCJ,ELine=@counterrline
from ErrorRecords JOIN ErrorData ON ErrorRecords.������=ErrorData.������ 
where ErrorRecords.��¼���=@ErrorCodejl

set @intErrorCode = 3
goto ERR_HANDLER
END /*17C53*/ --2e
else
BEGIN --3b
if (@ErrorCode = 201990 OR @ErrorCode=204990 OR @ErrorCode=203990 OR @ErrorCode=206990 OR @ErrorCode=207990 OR @ErrorCode=215990 OR @ErrorCode=216990 )
begin --4b
Select ErrorRecords.��¼���,ErrorRecords.������,ErrorData.�۷�����
from ErrorRecords JOIN ErrorData ON ErrorRecords.������=ErrorData.������ 
where ErrorRecords.��¼���=@ErrorCodejl

set @intErrorCode = 5
goto ERR_HANDLER
end --4e
else if((@ErrorCode>201500 and  @ErrorCode<201700) OR (@ErrorCode>203500 and  @ErrorCode<203700) OR (@ErrorCode>204500 and  @ErrorCode<204700)OR (@ErrorCode>206500 and  @ErrorCode<206700) OR (@ErrorCode>207500 and  @ErrorCode<207700)
OR (@ErrorCode>214500 and  @ErrorCode<214700)OR (@ErrorCode>215500 and  @ErrorCode<215700) OR (@ErrorCode>216500 and  @ErrorCode<216700))
begin --5b
Select ErrorRecords.��¼���,ErrorRecords.������,ErrorData.�۷�����
from ErrorRecords JOIN ErrorData ON ErrorRecords.������=ErrorData.������ 
where ErrorRecords.��¼���=@ErrorCodejl
set @intErrorCode = 2
goto ERR_HANDLER
end --5e
else
begin --6b
set @intErrorCode = 0
goto ERR_HANDLER
end --6e
END /*Not 17C53*/ --3e
end --1e
else
begin --7b
if(@Status =1 OR @Status =2)
BEGIN
	Select ״̬=@Status,�ɼ�=@KSCJ,S=127,E=127
	Set @intErrorCode =6
	goto ERR_HANDLER
END
else
BEGIN --8 zt!=1 or zt!=2
	Select ״̬=@Status
	Set @intErrorCode=-1
END --8 zt!=1 or zt!=2
end	--7e

ERR_HANDLER:
	return @intErrorCode
END
/*2017-10-02 ZSZ ����:׼��֤�����,���¼���,����ֵ*/
GO
